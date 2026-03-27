# Issue: Runtime and memory-safety bugs in string and arena paths

## Summary
Several bugs in string buffer growth, split parsing, and arena-list allocation can cause:
- garbage output from string buffers
- out-of-bounds writes
- incorrect assert behavior on invalid input
- allocation failures or crashes for large arena-list requests

## Affected areas
- src/string.c
- src/arena.c
- includes/lib3man.h
- src/dynarray.c

## Testing and build context
- main.c was updated as a local test harness to reproduce and validate edge cases.
- Makefile now includes stricter/debug flags: -Werror -g -fsanitize=address.
- The main.c and Makefile changes are test/tooling context, not runtime library API changes.

## User-visible symptoms
- Creating a string buffer from a C string can print unexpected bytes.
- Repeated pushes can reallocate with the wrong size and corrupt memory.
- Converting a string buffer to C string can write beyond capacity when len equals cap.
- Split logic can read beyond used bytes and has an unsafe loop condition.
- Arena-list allocation can fail for large requests and can overwrite existing next nodes.

## Root causes
1. sb_from_cstr allocated memory but did not copy the source bytes.
2. Multiple push functions used realloc with the old capacity size instead of the computed new capacity.
3. Capacity checks used strict greater-than where greater-than-or-equal is required.
4. sb_split_svs_char iterated over cap instead of len and used an incorrect loop condition when filling output views.
5. cstr_from_sb wrote a null terminator without confirming available capacity.
6. sb_arenaList_push_* asserts used OR logic that allowed invalid pointers.
7. arenaList_Alloc did not traverse list safely and did not guarantee new arena capacity can satisfy the requested size.
8. Macro-based dynamic-array API in the header was hard to maintain and type-specific.

## Why this fix strategy
- Keep API and behavior stable while making internals safe.
- Use minimal targeted changes to reduce regression risk.
- Prefer deterministic growth loops so capacity always fits requested payload.
- Validate inputs early and fail safely.
- Replace the macro DA approach with one generic runtime API for clarity and maintainability.

## Fixes applied
- Reworked split logic to operate only on used bytes and to return total split count reliably.
- Copied input bytes in sb_from_cstr and ensured minimum capacity is at least 1.
- Corrected growth checks and reallocation sizes in all push paths.
- Added invalid-buffer guard in sb_push_char.
- Added capacity guard in cstr_from_sb before writing null terminator.
- Corrected asserts in arena-list push helpers to require valid inputs.
- Reworked arenaList_Alloc to scan existing nodes first, allocate a new node only when needed, and size the new arena to at least the requested allocation size.
- Replaced macro-based DA declarations in the header with a generic DynArray API.
- Added src/dynarray.c implementing da_new, da_push, and da_at.

## Validation
- Build: make clean && make
- Runtime: ./main
- Result: successful build and output prints hello with no AddressSanitizer crash in active main test.

## DynArray migration details
- Removed CREATE_ARR macro usage surface from includes/lib3man.h.
- Added generic struct:
	- void *data
	- size
	- capacity
	- elem_size
- Added generic API:
	- da_new(size_t elem_size, size_t capacity)
	- da_push(DynArray *da, const void *value)
	- da_at(DynArray *da, size_t index)

## Risk and follow-up
- arenaList_Realloc still assumes head-arena cursor comparison and may need a separate pass for strict correctness across multi-node lists.
- Additional unit tests are recommended for split edge cases and arena-list realloc behavior.

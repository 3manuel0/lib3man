# PR: Fix runtime and memory-safety issues in string and arena internals

## Scope note
- main.c changes are test scenarios used to reproduce and validate fixes.
- Makefile flag changes (-Werror -g -fsanitize=address) are build-safety improvements for verification.
- Core functional library changes are in src/string.c, src/arena.c, includes/lib3man.h, and src/dynarray.c.

## What changed
### src/string.c
- Reimplemented sb_split_svs_char parsing logic to:
  - count delimiters only across sb len bytes
  - return total number of parts consistently
  - safely fill output views without unsafe loop conditions
- Updated sb_from_cstr to copy source bytes into the allocated buffer.
- Ensured sb_from_cstr capacity is at least 1 for empty input.
- Fixed growth logic in:
  - sb_push_sv
  - sb_push_cstr
  - sb_push_cstr_sz
  - sb_arenaList_push_cstr_sz
  - sb_arenaList_push_sv
- Replaced strict capacity checks with inclusive checks where needed.
- Added guard in sb_push_char for invalid buffer state and corrected realloc size argument.
- Added bounds guard in cstr_from_sb to prevent null-terminator overflow.
- Added concise comments in modified sections to explain safety and growth decisions.
- Removed ternary operators in touched logic.

### src/arena.c
- Reworked arenaList_Alloc to:
  - assert valid input
  - scan existing arena-list nodes before allocating new nodes
  - avoid overwriting existing next links
  - allocate a new arena sized to at least the requested allocation
  - return NULL on node allocation failure instead of exiting process
- Added concise comments around allocation-path behavior.

### includes/lib3man.h
- Removed the macro-based dynamic array declaration block.
- Added generic DynArray type and public API declarations:
  - da_new
  - da_push
  - da_at

### src/dynarray.c
- Added implementation of the generic dynamic-array API.
- Uses memcpy-based element insertion and geometric capacity growth.
- Keeps behavior predictable for zero/invalid capacity and out-of-bounds access.

## Why these changes were made this way
- Preserve public API while fixing unsafe internals.
- Minimize scope to the failing paths observed from main-based repros.
- Guarantee that every grow path computes a capacity that actually fits incoming data.
- Prevent out-of-bounds writes and assert misuse with explicit guards.
- Make arena-list allocation resilient for requests larger than base arena capacity.
- Keep maintenance cost low by adding small focused comments near non-obvious logic.
- Improve maintainability by replacing macro-generated DA APIs with a single generic implementation.

## Verification
- Ran: make clean && make && ./main
- Result: build succeeds and active main test prints hello.

## Out of scope
- main.c and Makefile are intentionally used for testing/verification context and may be adjusted independently from library API changes.

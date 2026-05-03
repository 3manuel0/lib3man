## Lib3man

Lib3man is a small C library I'm building for learning purposes and to use in my other C projects.

### It has:

- #### `Arena Allocator` :
  ![string_view.svg](images/arena_allocator.svg)
- #### `Length-based string (string view)` :
  ![string_view.svg](images/string_view.svg)
- #### `Dynamically allocated strings (string buffer)` :
  ![string_view.svg](images/string_buffer.svg)
- #### `Matrix (Math Matrix)`
- #### `Utilities (os based pseudo-random)`

### Project Structure

```text
.
├── includes
│   └── lib3man.h   # Unified header file
├── lib
│   ├── lib3man.a   # Static library (Mingw (windows), gcc, clang)
│   └── lib3man.lib # Static library (msvc)
│
├── main.c          # Testing the library
│
└── src
    ├── arena.c     # Memory Arenas and Arena-list
    ├── matrix.c    # String view, String buffer
    ├── string.c    # Matrix
    └── utility.c   # OS based pseudo-random

```

### Goals

- Explore low-level C design, memory management, math etc...
- Build a personal "Standard Library" for my other projects.
- Having fun while also understanding complex concepts.

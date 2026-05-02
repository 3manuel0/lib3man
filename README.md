## Lib3man

Lib3man is a small C library I'm building for learning purposes and for use in my other C projects.

It includes some basic data structures :

- Memory arenas
- Arena-list (for managing multiple arenas)
- Length-based string (string view)
- Dynamically allocated strings (string buffer)
- Matrix (Math Matrix)
- Utilities (os based pseudo-random)

### Project Structure

```text
.
├── src/
│   ├── arena.c    # Memory Arenas and Arena-list
│   ├── string.c   # String view, String buffer
│   ├── matrix.c   # Matrix
│   └── utility.c  # os based pseudo-random
└── include/
    └── lib3man.h      # Unified header
```

### Goals

The goal of this library is to explore low-level C design, memory management, math, and create a useful library to use for my other projects.

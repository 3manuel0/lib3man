CC      = gcc
AR      = ar
CFLAGS  = -Wall -Wextra -Iincludes
ARFLAGS = rcs

SRC = src/*.c
OBJ = $(SRC:.c=.o)
LIB = lib/lib3man.a

all: $(LIB) main

$(LIB): $(OBJ)
	$(AR) rcs $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

main:
	$(CC) $(CFLAGS) main.c -Llib -l3man -o main

clean:
	rm -f src/*.o $(LIB) main
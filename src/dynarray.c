#include "../includes/lib3man.h"

/*
 * Create a new dynamic array.
 * 
 * elem_size:        Size of each element in bytes.
 * initial_capacity: Starting capacity. If 0, a reasonable default (8) is used.
 */
DynArray da_new(size_t elem_size, size_t initial_capacity)
{
    DynArray da = {0};

    if (elem_size == 0)
        return da;

    da.elem_size = elem_size;

    // Use a reasonable default if no capacity is given
    if (initial_capacity == 0)
        initial_capacity = 8;

    da.data = malloc(initial_capacity * elem_size);
    // allocation failed, capacity stays 0
    if (da.data == NULL)
        return da;

    da.capacity = initial_capacity;
    da.size = 0;

    return da;
}

/*
 * Append an element to the dynamic array.
 * The element is copied using memcpy.
 */
void da_push(DynArray *da, const void *value)
{
    assert(da != NULL);
    assert(value != NULL);
    assert(da->elem_size > 0);

    // Grow the array if we reached current capacity
    if (da->size >= da->capacity) {
        size_t new_capacity;

        if (da->capacity == 0)
            new_capacity = 8;
        else
            new_capacity = da->capacity * 2;

        void *new_data = realloc(da->data, new_capacity * da->elem_size);
        // realloc failed, do nothing
        if (new_data == NULL)
            return;

        da->data = new_data;
        da->capacity = new_capacity;
    }

    // Copy the new element into the next free slot
    memcpy((char *)da->data + (da->size * da->elem_size),
           value,
           da->elem_size);

    da->size++;
}

/*
 * Return pointer to element at given index.
 * Returns NULL if index is out of bounds.
 */
void *da_at(DynArray *da, size_t index)
{
    assert(da != NULL);

    if (index >= da->size)
        return NULL;

    return (char *)da->data + (index * da->elem_size);
}
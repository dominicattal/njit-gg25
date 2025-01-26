#include "array.h"
#include <stdlib.h>

Array* array_create(void)
{
    Array* arr = malloc(sizeof(Array));
    arr->buffer = NULL;
    arr->length = arr->capacity = 0;
    return arr;
}

void array_push(Array* arr, void* element)
{
    if (arr->length >= arr->capacity) {
        arr->capacity += 10;
        if (arr->buffer == NULL)
            arr->buffer = malloc(arr->capacity * sizeof(void*));
        else
            arr->buffer = realloc(arr->buffer, arr->capacity * sizeof(void*));
    }
    arr->buffer[arr->length++] = element;
}

void* array_pop(Array* arr, i32 idx)
{
    void* element = arr->buffer[idx];
    arr->buffer[idx] = arr->buffer[--arr->length];
    if (arr->length == 0) {
        free(arr->buffer);
        arr->buffer = NULL;
        arr->capacity = 0;
    }
    return element;
}

void* array_get(Array* arr, i32 idx)
{
    return arr->buffer[idx];
}

bool array_empty(Array* arr)
{
    return arr->length == 0;
}

void array_destroy(Array* arr)
{
    free(arr->buffer);
    free(arr);
}
#ifndef ARRAY_H
#define ARRAY_H

#include "type.h"

typedef struct {
    i32 length;
    i32 capacity;
    void** buffer;
} Array;

Array* array_create(void);
void   array_push(Array* arr, void* element);
void*  array_pop(Array* arr, i32 idx);
void*  array_get(Array* arr, i32 idx);
bool   array_empty(Array* arr);
void   array_destroy(Array* arr);

#endif
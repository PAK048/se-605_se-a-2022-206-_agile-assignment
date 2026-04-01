#ifndef DYNARRAY_H
#define DYNARRAY_H

struct DynArray {
    int* data;
    int count;
    int capacity;
};

void initArray(DynArray& da, int initialCapacity);
void autoGrow(DynArray& da);
void push(DynArray& da, int value);
int pop(DynArray& da);
void freeArray(DynArray& da);

#endif
#include "dynarray.h"

void initArray(DynArray& da, int initialCapacity) {
    da.data = new int[initialCapacity];
    da.count = 0;
    da.capacity = initialCapacity;
}

// TODO: Implement autoGrow
// When the array is full, allocate a new array with DOUBLE the capacity,
// copy all existing elements to the new array, delete the old array,
// and update da.data and da.capacity.
void autoGrow(DynArray& da) {
    // --- Write your code here ---

}

void push(DynArray& da, int value) {
    if (da.count == da.capacity) {
        autoGrow(da);
        if (da.count == da.capacity) {
            return;
        }
    }
    da.data[da.count++] = value;
}

int pop(DynArray& da) {
    if (da.count == 0) return -1;
    return da.data[--da.count];
}

void freeArray(DynArray& da) {
    delete[] da.data;
    da.data = nullptr;
    da.count = 0;
    da.capacity = 0;
}
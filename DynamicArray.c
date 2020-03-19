#include "DynamicArray.h"

/*
Initializes a DynamicArray of DEFAULT_ARRAYLIST_SIZE elements
O: a DynamicArray to initialize
I: def default value of all the array's elements
*/
void DynamicArrayInit(DynamicArray *a, int def){
    a->size = DEFAULT_ARRAYLIST_SIZE;
    a->content = calloc(a->size, sizeof(int));
    if(def)
        DynamicArrayFill(a, 0, a->size, def);
}

/*
Adds an element to a DynamicArray
O: a DynamicArray to which add the element elem
I: index index at which add the element elem
I: elem element to add at the index specified by index
*/
void DynamicArrayAdd(DynamicArray *a, long index, long long elem){
    int prevSize = a->size;
    if(index >= a->size){
        a->size *= 2;
        a->content = (int *)realloc(a->content, a->size * sizeof(int));
    }
    DynamicArrayFill(a, prevSize, a->size, -1);
    a->content[index] = elem;
}

/*
Get an element of the DynamicArray at index
I: a DynamicArray from which get the element
I: index index from which get the element
O: ret element at index index in the DynamicArray a
Return: true(1) if it was possible to add the element to the DynamicArray, false(0) otherwise
*/

int DynamicArrayGet(const DynamicArray *a, long index, long long *ret){
    if(index >= 0){
        *ret = a->content[index];
        return true;
    }else{
        return false;
    }
}

/*
Fills the DynamicArray with the value specified by val
O: a DynamicArray to fill
I: from index from where to start filling the Dynamic array
I: to index at which to stop filling the DynamicArray
I: val value to fill the DynamicArray with
*/
void DynamicArrayFill(DynamicArray *a, int from, int to, int val){
    int i;
    for(i = from; i < to; i++)
        a->content[i] = val;
}

/*
Frees the DynamicArray
O: a DynamicArray to free
*/
void freeDynamicArray(DynamicArray *a){
    free(a->content);
    a->content = NULL;
    a->size = 0;
}

#include <stdlib.h>
#include <stdbool.h>
#ifndef _DYNAMICARRAY
#define _DYNAMICARRAY
#define DEFAULT_ARRAYLIST_SIZE 20

typedef struct {
  int *content;
  size_t size;
} DynamicArray;

void DynamicArrayInit(DynamicArray*, int);
void DynamicArrayAdd (DynamicArray*, long, long long);
int  DynamicArrayGet (const DynamicArray*, long, long long*);
void DynamicArrayFill(DynamicArray*, int, int, int);
void freeDynamicArray(DynamicArray*);
#endif

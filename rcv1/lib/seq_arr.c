#include "seq_arr.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static 
const size_t MIN_SIZE = 8; 

static
void maybe_expand(seq_arr_t* arr)
{
  if(arr->size + 1 == arr->cap){
    //void* p = reallocarray(arr->data, arr->cap*2, arr->elt_size); 
    arr->data = realloc(arr->data, 2*arr->cap*arr->elt_size);
    assert(arr->data != NULL && "realloc failed to allocate memory");
    arr->cap *=2;
  }
}

static
void maybe_shrink(seq_arr_t* arr)
{
  const float occ = (float)arr->size / (float)arr->cap; 
  //printf("Occ = %f and size = %ld \n", occ, arr->size);
  if(arr->size > MIN_SIZE && occ < 0.25){
    assert(arr->cap > MIN_SIZE); 
    seq_arr_t tmp = {.data = NULL, .size = arr->size, .elt_size = arr->elt_size, .cap = arr->cap/2};
    tmp.data = calloc(tmp.cap, tmp.cap);
    assert(tmp.data != NULL && "Memory exhausted");
    assert(arr->size <= tmp.cap);
    memcpy(tmp.data, arr->data, arr->size*arr->elt_size);
    free(arr->data);
    memcpy(arr, &tmp, sizeof(seq_arr_t));
  }
}

void seq_arr_init(seq_arr_t* arr, size_t elt_size) //__attribute__(malloc)
{
  assert(arr != NULL);
  seq_arr_t tmp = {.data = NULL, .size = 0, .elt_size = elt_size, .cap = MIN_SIZE};
  memcpy(arr, &tmp, sizeof(seq_arr_t));
  arr->data = calloc(arr->cap, elt_size);
  assert(arr->data != NULL);
}

void seq_arr_free(seq_arr_t* arr, void (*free_func)(void*) )
{
  assert(arr != NULL);
  assert(arr->data != NULL);

  if(free_func != NULL) {
    void* start_it = seq_arr_front(arr);
    void* end_it = seq_arr_end(arr);
    while(start_it != end_it){
      free_func(start_it); 
      start_it = seq_arr_next(arr, start_it);
    }
  }

  free(arr->data);
}

void seq_arr_push_back(seq_arr_t* arr, void* data, size_t len)
{
  assert(arr != NULL);
  //assert(data != NULL);
  assert(len == arr->elt_size);
  
  maybe_expand(arr);

  const size_t offset = arr->size * arr->elt_size; 
  memcpy(&arr->data[offset], data, arr->elt_size);
  arr->size += 1;
};

void seq_arr_erase(seq_arr_t* arr, void* start_it, void* end_it)
{
  // start_it && end_it must be in the range of arr->data
  assert(arr != NULL);
  assert(start_it != NULL);
  assert(end_it != NULL);
  assert(end_it >= start_it); 

  if(start_it == end_it) return;
  
  const int num_bytes_move = seq_arr_end(arr) - end_it;
  assert(num_bytes_move > -1);
  memmove(start_it, end_it, num_bytes_move); 

  const int num_bytes_erase = end_it - start_it;
  const int32_t num_elm_erase = num_bytes_erase/arr->elt_size;
  assert(num_elm_erase > 0);
  arr->size -= num_elm_erase;

  memset(seq_arr_end(arr), 0, num_bytes_erase);

  maybe_shrink(arr);
}

/*
void seq_arr_erase_free(seq_arr_t *arr, void *start_it, void *end_it,
                        void (*free_func)(void *)) {
  assert(arr != NULL);
  assert(start_it != NULL);
  assert(end_it != NULL);
  assert(end_it >= start_it);
  assert(free_func != NULL && "Use seq_arr_erase instead ");

  while (start_it != end_it) {
    free_func(start_it);
    start_it = seq_arr_next(arr, start_it);
  }

  seq_arr_erase(arr, start_it, end_it);
}
*/

size_t seq_arr_size(seq_arr_t* arr)
{
  assert(arr != NULL);
  return arr->size;
}

void* seq_arr_front(seq_arr_t* arr)
{
  return arr->data;
}

void* seq_arr_next(seq_arr_t* arr, void* it)
{
  assert(arr != NULL);
  assert(it != NULL);
  return (uint8_t*)it + arr->elt_size;
}

void* seq_arr_end(seq_arr_t* arr)
{
  assert(arr != NULL);
  return &arr->data[arr->size*arr->elt_size];
}

void* seq_arr_at(seq_arr_t* arr, uint32_t pos)
{
  assert(pos < arr->size);
  return arr->data + pos*arr->elt_size;
}

ptrdiff_t seq_arr_dist(seq_arr_t* arr, void* first, void* last)
{
  assert(arr != NULL);
  assert(first != NULL);
  assert(last != NULL);
  const ptrdiff_t pos = (last - first)/arr->elt_size;
  assert(pos > -1);
  return pos;
}

bool seq_arr_equal(seq_arr_t* arr, void* it_1, void* it_2)
{
  assert(arr!= NULL);
  assert(it_1 != NULL);
  assert(it_2 != NULL);
  return it_1 == it_2;
}

void* seq_arr_value(seq_arr_t* arr, void* it)
{
  assert(arr != NULL);
  assert(it != NULL);

  assert(0!=0 && "Not implemented!"); 
  return NULL;
}


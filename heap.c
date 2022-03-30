//
// Created by fitli on 30.03.22.
//

#include <stdlib.h>
#include <stdio.h>
#include "heap.h"

struct Heap create_empty_heap(int max_size, int (*cmp_func)(void*, void*)) {
    struct Heap new;
    new.max_size = max_size;
    new.num_elems = 0;
    new.cmp_func = cmp_func;
    new.memory = malloc(max_size * sizeof(void *));
    if (new.memory == NULL) {
        fprintf(stderr, "Allocation error\n");
    }
    return new;
}

int add_element_heap(struct Heap *heap, void *elem) {
    if (heap->num_elems + 1 == heap->max_size) {
        void **moved = realloc(heap->memory, heap->max_size*2*sizeof(void*));
        if (moved == NULL) {
            fprintf(stderr, "Allocation error\n");
            return 0;
        }
        heap->memory = moved;
        heap->max_size *= 2;
    }
    int pos = heap->num_elems + 1;
    int parent = pos/2;
    while(pos > 1 && heap->cmp_func(elem, heap->memory[parent]) > 0) {
        heap->memory[pos] = heap->memory[parent];
        pos = parent;
        parent /= 2;
    }
    heap->memory[pos] = elem;
    heap->num_elems++;
    return 1;
}

void *pop_heap(struct Heap *heap) {
    void* ret = heap->memory[1];
    int pos = 1;
    void *last_elem = heap->memory[heap->num_elems];
    int next;
    while(2*pos <= heap->num_elems) {
        if (2*pos == heap->num_elems || heap->cmp_func(heap->memory[2*pos], heap->memory[2*pos + 1]) >= 0) {
            next = 2*pos;
        }
        else {
            next = 2*pos + 1;
        }
        if(heap->cmp_func(heap->memory[next], last_elem) <= 0) {
            break;
        }
        heap->memory[pos] = heap->memory[next];
        pos = next;
    }
    heap->memory[pos] = last_elem;
    heap->num_elems--;
}
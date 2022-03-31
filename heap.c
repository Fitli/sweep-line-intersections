//
// Created by fitli on 30.03.22.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "heap.h"


void push_down(struct Heap *heap, int pos) {
    void *elem = heap->memory[pos];
    int next;
    while(2*pos <= heap->num_elems) {
        if (2*pos == heap->num_elems || heap->cmp_func(heap->memory[2*pos], heap->memory[2*pos + 1]) >= 0) {
            next = 2*pos;
        }
        else {
            next = 2*pos + 1;
        }
        if(heap->cmp_func(heap->memory[next], elem) <= 0) {
            break;
        }
        heap->memory[pos] = heap->memory[next];
        pos = next;
    }
    heap->memory[pos] = elem;
}

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

struct Heap heapify(void** array, int len, int (*cmp_func)(void*, void*)) {
    struct Heap heap;
    heap.max_size = 1;
    while(heap.max_size <= len) {
        heap.max_size *= 2;
    }
    heap.memory = malloc(heap.max_size * sizeof(void *));
    if (heap.memory == NULL) {
        fprintf(stderr, "Allocation error\n");
    }
    heap.num_elems = len;
    heap.cmp_func = cmp_func;
    memcpy(heap.memory+1, array, len*sizeof (void *));
    for(int start_pos = heap.max_size/4; start_pos > 0; start_pos /= 2) {
        for(int pos = start_pos; pos < start_pos*2; pos++) {
            push_down(&heap, pos);
        }
    }
    return heap;
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
    heap->memory[1] = heap->memory[heap->num_elems];
    heap->num_elems--;
    push_down(heap, 1);
    return ret;
}

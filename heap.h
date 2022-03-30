//
// Created by fitli on 30.03.22.
//

#ifndef SWEEP_LINE_INTERSECTIONS_HEAP_H
#define SWEEP_LINE_INTERSECTIONS_HEAP_H

struct Heap {
    int max_size;
    int num_elems;
    int (*cmp_func)(void*, void*);
    void **memory;
};

struct Heap create_empty_heap(int max_size, int (*cmp_func)(void*, void*));
int add_element_heap(struct Heap *heap, void *elem);
void *pop_heap(struct Heap *heap);

#endif //SWEEP_LINE_INTERSECTIONS_HEAP_H

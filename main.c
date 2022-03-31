#include <stdio.h>
#include "heap.h"

int cmp_int(void *a, void *b) {
    return *((int*) a) - *((int *) b);
}

void print_heap(struct Heap heap) {
    printf("[");
    for (int i=1; i<=heap.num_elems; i++) {
        printf("%d ", *((int*) heap.memory[i]));
    }
    printf("]\n");
}

int main() {
    printf("create heap\n");
    struct Heap heap = create_empty_heap(4, &cmp_int);
    print_heap(heap);
    int numbers[10] = {3, 4, 7, 1, 2, 6, 3, 5, 9, 0};
    for(int i=0; i<10; i++) {
        printf("add %d\n", numbers[i]);
        add_element_heap(&heap, &numbers[i]);
        print_heap(heap);
    }
    for(int i=0; i<10; i++) {
        printf("pop\n");
        pop_heap(&heap);
        print_heap(heap);
    }

    int *pointers[10];
    for(int i=0; i<10; i++) {
        pointers[i] = &numbers[i];
    }
    printf("heapify\n");
    struct Heap heap2 = heapify((void *) pointers, 10, &cmp_int);
    print_heap(heap2);
    return 0;
}

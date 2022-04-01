#include <stdio.h>
#include "heap.h"
#include "segment.h"
#include "binsearchtree.h"

int cmp_int(void *a, void *b) {
    return *((int*) a) - *((int *) b);
}
int cmp_int_tmp(void *a, void *b, void* tmp) {
    return *((int*) a) - *((int *) b);
}

void print_heap(struct Heap heap) {
    printf("[");
    for (int i=1; i<=heap.num_elems; i++) {
        printf("%d ", *((int*) heap.memory[i]));
    }
    printf("]\n");
}

void print_tree(struct BSTNode *root) {
    if(root == NULL) {
        printf("- ");
        return;
    }
    print_tree(root->left);
    printf("%d ", *(int*) root->data);
    print_tree(root->right);
}

void test_tree() {
    printf("create tree\n");
    struct BST tree = create_empty_bst(&cmp_int_tmp);
    print_tree(tree.root);
    printf("\n");

    int numbers[10] = {3, 4, 7, 1, 2, 6, 3, 5, 9, 0};
    for(int i=0; i<10; i++) {
        printf("add %d\n", numbers[i]);
        add_node(&tree, &numbers[i], NULL);
        print_tree(tree.root);
        printf("\n");
    }


    int a = 4;
    printf("find 4\n");
    struct BSTNode *node = find_node(&tree, &a, NULL);
    node == NULL ? printf("NULL\n") : printf("%d\n", *(int*) node->data);

    int b = 8;
    printf("find 8\n");
    struct BSTNode *node2 = find_node(&tree, &b, NULL);
    node2 == NULL ? printf("NULL\n") : printf("%d\n", *(int*) node2->data);

    printf("delete 4\n");
    remove_node(&tree, node);
    print_tree(tree.root);
    printf("\n");
}

void test_heap() {
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
}

void test_intersection () {
    struct Segment a, b;
    /*a.start.type = START;
    a.start.x = 1;
    a.start.y = 1;
    a.end.type = END;
    a.end.x = 5;
    a.end.y = 3;
    b.start.type = START;
    b.start.x = 2;
    b.start.y = 3;
    b.end.type = END;
    b.end.x = 5;
    b.end.y = 0;*/

    /*a.start.type = START;
    a.start.x = 1;
    a.start.y = 2;
    a.end.type = END;
    a.end.x = 3;
    a.end.y = 1;
    b.start.type = START;
    b.start.x = 2;
    b.start.y = 3;
    b.end.type = END;
    b.end.x = 5;
    b.end.y = 1;*/

    a.start.type = START;
    a.start.x = 1;
    a.start.y = 0;
    a.end.type = END;
    a.end.x = 2;
    a.end.y = 1;
    b.start.type = START;
    b.start.x = 2;
    b.start.y = 3;
    b.end.type = END;
    b.end.x = 5;
    b.end.y = 0;

    struct Point insct = intersection(a, b);
    printf("intersectin[%d, %d], type %d\n", insct.x, insct.y, insct.type);
}

int main() {
    test_tree();

    return 0;
}

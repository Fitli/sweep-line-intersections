//
// Created by fitli on 01.04.22.
//

#ifndef SWEEP_LINE_INTERSECTIONS_TESTS_H
#define SWEEP_LINE_INTERSECTIONS_TESTS_H

#include "heap.h"
#include "segment.h"
#include "binsearchtree.h"

void test_intersection ();
void test_heap();
void test_tree();
void print_tree(struct BSTNode *root);
void print_tree_segments(struct BSTNode *root);
void print_heap(struct Heap heap);
int cmp_int_tmp(void *a, void *b, void* tmp);
int cmp_int(void *a, void *b);

#endif //SWEEP_LINE_INTERSECTIONS_TESTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "segment.h"
#include "binsearchtree.h"
//#include "rbtree.h"
//#include "rst.h"
#include "tests.h"

int points_by_x(void *a, void *b) {
    struct Point* point_a = (struct Point*) a;
    struct Point* point_b = (struct Point*) b;
    if(point_a->x == point_b->x) {
        if (point_a->y <= point_b->y) {
            return 1;
        }
        else {
            return -1;
        }
    } else if(point_a->x < point_b->x) {
        return 1;
    } else if (point_a->x > point_b->x) {
        return -1;
    }
    return 0;
}

int segments_by_y(void *a, void *b, void *x) {
    double a_at_x = y_at_x((struct Segment *) a, *(int *) x);
    double b_at_x = y_at_x((struct Segment *) b, *(int *) x);
    if (a_at_x > b_at_x)
        return 1;
    else if (a_at_x > b_at_x)
        return -1;
    return 0;
}

void check_intersection(double x_position, struct Segment* seg1, struct Segment* seg2,
                        struct Heap *point_heap) {
    struct Point *intersec = malloc(sizeof(struct Point));
    *intersec = intersection(seg1, seg2);
    if(intersec->type == INTERSECTION && intersec->x >= x_position) {
        add_element_heap(point_heap, intersec);
    }
    else {
        free(intersec);
    }
}

int load_input(const char* filename, struct Segment **segments) {
    FILE *file = fopen(filename, "r");

    int num_segments = 0;
    fscanf(file, " %d", &num_segments);

    *segments = malloc(num_segments * sizeof(struct Segment));

    for(int i = 0; i<num_segments; i++) {
        double x1, y1, x2, y2;
        fscanf(file," %lf %lf %lf %lf", &x1, &y1, &x2, &y2);
        if(x1<x2) {
            (*segments)[i].start.x = x1;
            (*segments)[i].start.y = y1;
            (*segments)[i].end.x = x2;
            (*segments)[i].end.y = y2;
        } else {
            (*segments)[i].start.x = x2;
            (*segments)[i].start.y = y2;
            (*segments)[i].end.x = x1;
            (*segments)[i].end.y = y1;
        }

        (*segments)[i].start.type = START;
        (*segments)[i].start.segments_covered[0] = &(*segments)[i];
        (*segments)[i].end.type = END;
        (*segments)[i].end.segments_covered[0] = &(*segments)[i];
    }
    fclose(file);

    return num_segments;
}

double random_double(double min, double max) {
    int div = RAND_MAX / (max-min);
    return min + rand()*1.0 / div;
}

struct Segment *generate_random_segments(int num_segments) {
    struct Segment *segments = malloc(num_segments*sizeof(struct Segment));
    for(int i = 0; i < num_segments; i++) {
        double x1 = random_double(0, 1000);
        double x2 = random_double(0, 1000);
        double y1 = random_double(0, 1000);
        double y2 = random_double(0, 1000);
        if(x1 < x2) {
            segments[i].start.x = x1;
            segments[i].end.x = x2;
        } else {
            segments[i].start.x = x2;
            segments[i].end.x = x1;
        }
        segments[i].start.y = y1;
        segments[i].end.y = y2;
        segments[i].start.type = START;
        segments[i].end.type = END;
        segments[i].start.segments_covered[0] = &segments[i];
        segments[i].end.segments_covered[0] = &segments[i];
    }
    return segments;
}

void find_intersections(int num_segments, struct Segment *segments, int print_output, int print_stats) {
    //create priority queue with all the starts and ends of the segments
    void *point_pointers[num_segments * 2];

    for(int i = 0; i<num_segments; i++) {
        point_pointers[2*i] = &segments[i].start;
        point_pointers[2*i + 1] = &segments[i].end;
    }

    struct Heap point_heap = heapify(point_pointers, num_segments*2, &points_by_x);

    //create empty BST
    struct BST segmentBST = create_empty_bst(segments_by_y);

    // array for storing the intersections
    int intersect_cap = 10000000;
    int intersect_num = 0;
    struct Point *intersections = malloc(intersect_cap * sizeof(struct Point *));

    // main loop
    struct Point last_point;
    int first_run = 1;
    while(point_heap.num_elems > 0) {
        //pop point from the heap
        struct Point *point = pop_heap(&point_heap);

        // if the point is a duplicate of the previous point, continue
        if(first_run == 0 && (point->x == last_point.x) && (point->y == last_point.y)) {
            first_run = 0;
            last_point = *point;
            continue;
        }
        first_run = 0;
        last_point = *point;

        // if START add to the BST
        if (point->type == START) {
            struct BSTNode *node = add_node(&segmentBST, point->segments_covered[0], &point->x);
            if(!node) {
                return;
            }
            struct Segment *segment = point->segments_covered[0];
            segment->node = node;

            // check intersections with neighbours
            struct BSTNode *prev = find_prev(node, &segmentBST);
            if(prev != NULL)
                check_intersection(point->x, (struct Segment*) prev->data, (struct Segment*)node->data, &point_heap);

            struct BSTNode *next = find_next(node, &segmentBST);
            if(next != NULL)
                check_intersection(point->x, node->data, next->data, &point_heap);
        }
        // if END, remove from the BST
        else if (point->type == END) {
            struct BSTNode *node = point->segments_covered[0]->node;
            struct BSTNode *prev = find_prev(node, &segmentBST);
            struct BSTNode *next = find_next(node, &segmentBST);
            remove_node(&segmentBST, node);
            point->segments_covered[0]->node = NULL;

            // check intersection between neighbours of the removed segment
            if(prev != NULL && next != NULL)
                check_intersection(point->x, prev->data, next->data, &point_heap);
        }
        // if INTERSECTION, swap nodes in the BST
        else if (point->type == INTERSECTION) { // INTERSECTION
            struct BSTNode *first = point->segments_covered[0]->node;
            struct BSTNode *second = point->segments_covered[1]->node;

            // add intersection to the output array
            if (intersect_cap == intersect_num){
                intersect_cap *= 2;
                intersections = realloc(intersections, intersect_cap*sizeof(struct Point));
            }
            intersections[intersect_num] = *point;
            intersect_num++;

            //swab nodes
            swap_nodes(&segmentBST, first, second);

            //check newly created pairs of neighbours
            struct BSTNode *prev = find_prev(second, &segmentBST);
            if(prev != NULL)
                check_intersection(point->x, prev->data, second->data, &point_heap);

            struct BSTNode *next = find_next(first, &segmentBST);
            if(next != NULL)
                check_intersection(point->x, first->data, next->data, &point_heap);

            free(point);
        }

    }

    // print results
    if(print_output) {
        for(int i = 0; i<intersect_num; i++) {
            printf("%lf %lf\n", intersections[i].x, intersections[i].y);
        }
    }

    if(print_stats) {
        printf("%d, %d, %d, %d\n", segmentBST.num_comparisons, segmentBST.num_swaps, segmentBST.num_moves, point_heap.num_comparisons);
    }

    destroy_heap(&point_heap);
    free(intersections);
}

int main(int argc, char *argv[]) {
    struct Segment *segments;

    if(argc == 3 && strcmp(argv[1], "-i") == 0) {
        int num_segments = load_input(argv[2], &segments);
        find_intersections(num_segments, segments, 1, 0);
        free(segments);
    }
    else if(argc == 3 && strcmp(argv[1], "-r") == 0) {
        int num_segments = atoi(argv[2]);
        segments = generate_random_segments(num_segments);
        find_intersections(num_segments, segments, 0, 1);
        free(segments);
    }
    else {
        printf("Bad combination of arguments.\n");
        printf("USAGE:\n");
        printf("\t%s -i FILE\tprints intersections of segments in input file FILE\n", argv[0]);
        printf("\t%s -r N\t\truns the algorithm with a random input of N segments and returns stats\n", argv[0]);

    }

    return 0;
}

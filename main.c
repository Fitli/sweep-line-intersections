#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "segment.h"
//#include "binsearchtree.h"
#include "rbtree.h"
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

}

void find_intersections(const char* filename) {
    FILE *file = fopen(filename, "r");

    int num_segments = 4;
    fscanf(file, " %d", &num_segments);
    struct Segment segments[num_segments];
    void *point_pointers[num_segments * 2];
    for(int i = 0; i<num_segments; i++) {
        fscanf(file," %lf %lf %lf %lf", &segments[i].start.x, &segments[i].start.y, &segments[i].end.x, &segments[i].end.y);
        segments[i].start.type = START;
        segments[i].start.segments_covered[0] = &segments[i];
        segments[i].end.type = END;
        segments[i].end.segments_covered[0] = &segments[i];

        point_pointers[2*i] = &segments[i].start;
        point_pointers[2*i + 1] = &segments[i].end;
    }
    fclose(file);

    struct Heap point_heap = heapify(point_pointers, num_segments*2, &points_by_x);
    struct BST segmentBST = create_empty_bst(segments_by_y);

    int intersect_cap = 128;
    int intersect_num = 0;
    struct Point *intersections = malloc(intersect_cap * sizeof(struct Point *));

    struct Point last_point;
    int first_run = 1;
    while(point_heap.num_elems > 0) {
        struct Point *point = pop_heap(&point_heap);
        if(first_run == 0 && (point->x == last_point.x) && (point->y == last_point.y)) {
            first_run = 0;
            last_point = *point;
            continue;
        }
        first_run = 0;
        last_point = *point;

        if (point->type == START) {
            struct BSTNode *node = add_node(&segmentBST, point->segments_covered[0], &point->x);
            struct Segment *segment = point->segments_covered[0];

            printf("adding %lf %lf %lf %lf\n", point->segments_covered[0]->start.x, point->segments_covered[0]->start.y, point->segments_covered[0]->end.x, point->segments_covered[0]->end.y);

            segment->node = node;

            struct BSTNode *prev = find_prev(node);
            if(prev != NULL)
                check_intersection(point->x, (struct Segment*) prev->data, (struct Segment*)node->data, &point_heap);

            struct BSTNode *next = find_next(node);
            if(next != NULL)
                check_intersection(point->x, node->data, next->data, &point_heap);
        } else if (point->type == END) {
            struct BSTNode *node = point->segments_covered[0]->node;
            struct BSTNode *prev = find_prev(node);
            struct BSTNode *next = find_next(node);
            printf("removing %lf %lf %lf %lf\n", point->segments_covered[0]->start.x, point->segments_covered[0]->start.y, point->segments_covered[0]->end.x, point->segments_covered[0]->end.y);
            point->segments_covered[0]->node = NULL;
            remove_node(&segmentBST, node);
            if(prev != NULL && next != NULL)
                check_intersection(point->x, prev->data, next->data, &point_heap);
        } else { // INTERSECTION
            struct BSTNode *first = point->segments_covered[0]->node;
            struct BSTNode *second = point->segments_covered[1]->node;

            printf("found intersection %lf %lf\n", point->x, point->y);
            if (intersect_cap == intersect_num){
                intersect_cap *= 2;
                intersections = realloc(intersections, intersect_cap*sizeof(struct Point));
            }

            intersections[intersect_num] = *point;
            intersect_num++;

            swap_nodes(&segmentBST, first, second);

            struct BSTNode *prev = find_prev(second);
            if(prev != NULL)
                check_intersection(point->x, prev->data, second->data, &point_heap);

            struct BSTNode *next = find_next(first);
            if(next != NULL)
                check_intersection(point->x, first->data, next->data, &point_heap);
            free(point);
        }
    }

    for(int i = 0; i<intersect_num; i++) {
        printf("%lf %lf\n", intersections[i].x, intersections[i].y);
    }

    destroy_heap(&point_heap);
    free(intersections);
}

int main() {
    find_intersections("input.txt");
    //test_tree();

    return 0;
}

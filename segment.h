//
// Created by fitli on 31.03.22.
//

#ifndef SWEEP_LINE_INTERSECTIONS_SEGMENT_H
#define SWEEP_LINE_INTERSECTIONS_SEGMENT_H

#include "binsearchtree.h"

enum Type {
    START,
    END,
    INTERSECTION,
    INTERSECTION_INV
};

struct Point {
    int x;
    int y;
    enum Type type;
    struct Segment *segments_covered[2];
};

struct Segment {
    struct Point start;
    struct Point end;
    struct BSTNode *node;
};

struct Point intersection(struct Segment *a, struct Segment *b);
double y_at_x(const struct Segment *segment, int x);

#endif //SWEEP_LINE_INTERSECTIONS_SEGMENT_H

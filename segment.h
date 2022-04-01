//
// Created by fitli on 31.03.22.
//

#ifndef SWEEP_LINE_INTERSECTIONS_SEGMENT_H
#define SWEEP_LINE_INTERSECTIONS_SEGMENT_H

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
};

struct Segment {
    struct Point start;
    struct Point end;
};

struct Point intersection(struct Segment a, struct Segment b);

#endif //SWEEP_LINE_INTERSECTIONS_SEGMENT_H

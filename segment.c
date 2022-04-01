//
// Created by fitli on 31.03.22.
//

#include <math.h>
#include "segment.h"

struct Point intersection(struct Segment a, struct Segment b) {

    int abs_coef_x = a.start.x - b.start.x;
    int abs_coef_y = a.start.y - b.start.y;

    int diff_a_x = a.end.x - a.start.x;
    int diff_a_y = a.end.y - a.start.y;
    int diff_b_x = b.end.x - b.start.x;
    int diff_b_y = b.end.y - b.start.y;

    struct Point result;
    result.type = INTERSECTION;

    int denom = diff_b_y*diff_a_x - diff_a_y*diff_b_x;
    if(denom == 0) {
        result.type = INTERSECTION_INV;
        return result;
    }
    double a_mul = (1.0) * (abs_coef_y*diff_b_x - abs_coef_x*diff_b_y) / denom;
    result.x = (int) round(a.start.x + a_mul*diff_a_x);
    result.y = (int) round(a.start.y + a_mul*diff_a_y);

    if(result.x < a.start.x || result.x < b.start.x || result.x > a.end.x || result.x > b.end.x) {
        result.type = INTERSECTION_INV;
    }

    return result;
}
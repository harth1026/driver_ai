#ifndef CTMATH_H
#define CTMATH_H

#include "ctvec2.h"
#include "ctline.h"

float ctmath_distance(CTVEC2f obj_a, CTVEC2f obj_b);
double ctmath_angle(CTVEC2f obj_a, CTVEC2f obj_b);
double ctmath_relativeangle(CTVEC2f obj_a, CTVEC2f obj_b, double ang_a);
CTVEC2f ctmath_splitvector(double velocity, double angle);

double ctmath_linepointdistance(CTLINE line, CTVEC2f point);

#endif // CTMATH_H


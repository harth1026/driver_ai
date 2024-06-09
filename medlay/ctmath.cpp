#include <math.h>

#include "ctmath.h"

#define PI 					3.14159265


float ctmath_distance(CTVEC2f obj_a, CTVEC2f obj_b)
{
	return (float)sqrt(pow(obj_b.cx - obj_a.cx, 2) + pow(obj_b.cy - obj_a.cy, 2));
}

double ctmath_angle(CTVEC2f obj_a, CTVEC2f obj_b)
{
	double angle_ra = atan((obj_b.cy - obj_a.cy) / (obj_b.cx - obj_a.cx));
	double angle_dg = (angle_ra * 360.0f) / (2 * PI);

	return angle_dg; // degrees
}

double ctmath_relativeangle(CTVEC2f obj_a, CTVEC2f obj_b, double ang_a)
{
	double angle_ra = atan((obj_b.cy - obj_a.cy) / (obj_b.cx - obj_a.cx));
	double angle_dg = (angle_ra * 360.0f) / (2 * PI);
	double angle_rv = angle_dg - ang_a;
	while (angle_rv < 0)	angle_rv += 360;
	while (angle_rv > 360)	angle_rv -= 360;

	return angle_rv; // degrees
}

CTVEC2f ctmath_splitvector(double velocity, double angle)
{
	float velx = (float)(velocity * sin(angle * PI / 180.0f));
	float vely = (float)(-velocity * cos(angle * PI / 180.0f));
	return CTVEC2f(velx, vely);
}

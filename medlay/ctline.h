#ifndef CTLINE_H
#define CTLINE_H

#include <math.h>

#include "ctvec2.h"

class CTLINE
{
public:
	double slope;
	double offset;

	CTLINE() { slope = 0; offset = 0; }
	CTLINE(const CTLINE& b) { slope = b.slope; offset = b.offset; }
	CTLINE(double y, double o) { slope = y; offset = o; }
	CTLINE(CTVEC2f pos, double ang) 
	{
		slope = tan(ang * 3.14159 / 180.0);
		offset = pos.cx - (slope * pos.cy);
	}
	~CTLINE() {}

	CTLINE operator=(const CTLINE& b) { return CTLINE(this->slope = b.slope, this->offset = b.offset); }
	bool operator==(const CTLINE& b) { return (this->slope == b.slope) && (this->offset == b.offset); }
	bool operator!=(const CTLINE& b) { return (this->slope != b.slope) || (this->offset != b.offset); }

};

#endif // CTLINE_H


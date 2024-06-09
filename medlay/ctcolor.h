#ifndef CTCOLOR_H
#define CTCOLOR_H

#include <stdint.h>
class CTCOLOR
{
public:
	uint8_t r;
	uint8_t g;
	uint8_t b;

	CTCOLOR() { r = 0; g = 0; b = 0; }
	CTCOLOR(uint8_t ar, uint8_t ag, uint8_t ab) { r = ar; g = ag; b = ab; }
	CTCOLOR(const CTCOLOR& c) { r = c.r; g = c.g; b = c.b; }
};

#endif
 
#ifndef CTVEC2_H
#define CTVEC2_H

#include <math.h>

class CTVEC2f
{
public:
	float cx;
	float cy;

	CTVEC2f() { cx = 0; cy = 0; }
	CTVEC2f(const CTVEC2f& b) { cx = b.cx; cy = b.cy; }
	CTVEC2f(float x, float y) { cx = x, cy = y; }
	~CTVEC2f() {}

	CTVEC2f operator+(const CTVEC2f& b) { return CTVEC2f(this->cx + b.cx, this->cy + b.cy); }
	CTVEC2f operator+(const float b) { return CTVEC2f(this->cx + b, this->cy + b); }
	CTVEC2f operator-(const CTVEC2f& b) { return CTVEC2f(this->cx - b.cx, this->cy - b.cy); }
	CTVEC2f operator-(const float b) { return CTVEC2f(this->cx - b, this->cy - b); }
	CTVEC2f operator*(const float b) { return CTVEC2f(this->cx * b, this->cy * b); }
	CTVEC2f operator/(const float b) { return CTVEC2f(this->cx / b, this->cy / b); }
	CTVEC2f operator=(const CTVEC2f& b) { return CTVEC2f(this->cx = b.cx, this->cy = b.cy); }
	bool operator==(const CTVEC2f& b) { return (this->cx == b.cx) && (this->cy == b.cy); }
	bool operator!=(const CTVEC2f& b) { return (this->cx != b.cx) || (this->cy != b.cy); }
};

class CTVEC2
{
public:
	int cx;
	int cy;

	CTVEC2() { cx = 0; cy = 0; }
	CTVEC2(const CTVEC2& b) { cx = b.cx; cy = b.cy; }
	CTVEC2(const CTVEC2f& b) { cx = (int)b.cx; cy = (int)b.cy; }
	CTVEC2(int x, int y) { cx = x, cy = y; }
	CTVEC2(float x, float y) { cx = (int)x, cy = (int)y; }
	~CTVEC2() {}

	CTVEC2 operator+(const CTVEC2& b) { return CTVEC2(this->cx + b.cx, this->cy + b.cy); }
	CTVEC2 operator+(const int b) { return CTVEC2(this->cx + b, this->cy + b); }
	CTVEC2 operator-(const CTVEC2& b) { return CTVEC2(this->cx - b.cx, this->cy - b.cy); }
	CTVEC2 operator-(const int b) { return CTVEC2(this->cx - b, this->cy - b); }
	CTVEC2 operator*(const int b) { return CTVEC2(this->cx * b, this->cy * b); }
	CTVEC2 operator/(const int b) { return CTVEC2(this->cx / b, this->cy / b); }
	CTVEC2 operator=(const CTVEC2& b) { return CTVEC2(this->cx = b.cx, this->cy = b.cy); }
	bool operator==(const CTVEC2& b) { return (this->cx == b.cx) && (this->cy == b.cy); }
	bool operator!=(const CTVEC2& b) { return (this->cx != b.cx) || (this->cy != b.cy); }
};




#endif // CTVEC2

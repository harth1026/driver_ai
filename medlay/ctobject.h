#ifndef CTOBJECT_H
#define CTOBJECT_H

#include "ctvec2.h"

enum objectshape
{
	shape_rectangle = 0,
	shape_circle,
};

struct ctobject
{
	CTVEC2f		m_pos;		// current position
	CTVEC2f		m_vel;		// velocity
	CTVEC2f		m_acc;		// acceleration
	double		m_ang;		// angle (degrees)
	double		m_angvel;	// angular velocity / turning speed

	int			m_shape;
	double		m_dim1;		// square = x, circle = diameter
	double		m_dim2;		// square = y, circle = diameter
};

void ctobject_initialize(ctobject* so, int shape, double dim1, double dim2, CTVEC2f pos, double angle);
void ctobject_reset(ctobject* so, CTVEC2f pos, double angle);
void ctobject_update(ctobject* so);
void ctobject_updateall(ctobject* solist[], int objectcount);

void ctobject_setpos(ctobject* so, CTVEC2f pos);
void ctobject_setvel(ctobject* so, CTVEC2f vel);

bool ctobject_collision(ctobject* so1, ctobject* so2);

#endif // CTOBJECT_H

#include "vehicle.h"
#include <iterator>

#include "medlay/ctmath.h"

vehicle::vehicle()
{
	m_active = false;

	m_type_base = -1;

	m_so_body.m_pos = CTVEC2f(0, 0);
	m_so_body.m_vel = CTVEC2f(0, 0);
	m_so_body.m_acc = CTVEC2f(0, 0);		// acceleration
	m_so_body.m_ang = 0;
	m_so_body.m_angvel = 0;
	m_collision = false;

}

void vehicle_initialize(vehicle* a, int base, CTVEC2f pos, double angle)
{
	if (a)
	{
		a->m_active = true;
		ctobject_initialize(&a->m_so_body,   shape_rectangle, VEHICLE_SIZE_X, VEHICLE_SIZE_Y, pos, angle);
		a->m_collision = false;
		a->m_type_base = base;
	}
}

void vehicle_update(vehicle* a)
{
	if (a)
	{
		ctobject_update(&a->m_so_body);
	}
}

void vehicle_reset(vehicle* a, CTVEC2f pos, double angle)
{
	if(a)
	{	
		a->m_active = true;
		a->m_so_body.m_pos = pos;
		a->m_so_body.m_vel = CTVEC2f(0, 0);
		a->m_so_body.m_acc = CTVEC2f(0, 0);
		a->m_so_body.m_ang = angle;
		a->m_collision = false;
	}
}

void vehicle_movement(vehicle* a, VEHICLE_CONTROLS& c)
{
	switch (a->m_type_base)
	{
	case base_tank:
	{
		a->m_so_body.m_vel = ctmath_splitvector(5.0f * c.base_forward, a->m_so_body.m_ang);

		a->m_so_body.m_ang -= 5.0f * c.base_turn;
		if (a->m_so_body.m_ang < 0)
			a->m_so_body.m_ang += 360.0f;
	}
	break;

	case base_hover:
		a->m_so_body.m_acc = CTVEC2f((HOVER_ACC * c.base_turn), (-HOVER_ACC * c.base_forward));
		break;

	}
}

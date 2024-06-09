#include "target.h"

#include <iterator>

target::target()
{
	m_active = false;

	m_so_body.m_pos = CTVEC2f(0, 0);
	m_so_body.m_vel = CTVEC2f(0, 0);
	m_so_body.m_acc = CTVEC2f(0, 0);		// acceleration
	m_so_body.m_ang = 0;
	m_so_body.m_angvel = 0;
	m_collision = false;
}

void target_initialize(target* a, CTVEC2f pos)
{
	if (a)
	{
		a->m_active = true;
		ctobject_initialize(&a->m_so_body, shape_rectangle, TARGET_SIZE_X, TARGET_SIZE_Y, pos, 0);
		a->m_collision = false;
	}
}

void target_update(target* a)
{
	if (a)
	{
		ctobject_update(&a->m_so_body);
	}
}

void target_reset(target* a, CTVEC2f pos)
{
	if (a)
	{
		a->m_active = true;
		a->m_so_body.m_pos = pos;
		a->m_so_body.m_vel = CTVEC2f(0, 0);
		a->m_so_body.m_acc = CTVEC2f(0, 0);
		a->m_so_body.m_ang = 0;
		a->m_collision = false;
	}
}

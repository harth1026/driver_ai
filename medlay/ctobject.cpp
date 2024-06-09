#include "ctvec2.h"
#include "ctobject.h"

void ctobject_initialize(ctobject* so, int shape, double dim1, double dim2, CTVEC2f pos, double angle)
{
	if (so)
	{
		so->m_pos = pos;
		so->m_vel = CTVEC2f(0, 0);
		so->m_acc = CTVEC2f(0, 0);
		so->m_ang = angle;
		so->m_angvel = 0;
		so->m_shape = shape;
		so->m_dim1 = dim1;
		so->m_dim2 = dim2;
	}
}

void ctobject_reset(ctobject* so, CTVEC2f pos, double angle)
{
	if (so)
	{
		so->m_pos = pos;
		so->m_vel = CTVEC2f(0, 0);
		so->m_acc = CTVEC2f(0, 0);
		so->m_ang = angle;
		so->m_angvel = 0;
	}
}

void ctobject_update(ctobject* so)
{
	if(so)
	{
		so->m_vel = so->m_vel + so->m_acc;
		so->m_pos = so->m_pos + so->m_vel;
		so->m_ang += so->m_angvel;
		if (so->m_ang > 360)	so->m_ang -= 360;
		if (so->m_ang <   0)	so->m_ang += 360;
	}
}

void ctobject_updateall(ctobject* solist[], int objectcount)
{
	for (int i = 0; i < objectcount; i++)
	{
		ctobject_update(solist[i]);
	}
}

void ctobject_setpos(ctobject* so, CTVEC2f pos)
{
	if (so)
	{
		so->m_pos = pos;
	}
}

void ctobject_setvel(ctobject* so, CTVEC2f vel)
{
	if (so)
	{
		so->m_vel = vel;
	}
}


bool ctobject_collision(ctobject* so1, ctobject* so2)
{
	bool result = false;
	if (((so1->m_pos.cx + so1->m_dim1 / 2) > (so2->m_pos.cx - so2->m_dim1 / 2)) &&
		((so1->m_pos.cx - so1->m_dim1 / 2) < (so2->m_pos.cx + so2->m_dim1 / 2)))
	{
		if (((so1->m_pos.cy + so1->m_dim2 / 2) > (so2->m_pos.cy - so2->m_dim2 / 2)) &&
			((so1->m_pos.cy - so1->m_dim2 / 2) < (so2->m_pos.cy + so2->m_dim2 / 2)))
		{
			result = true;
		}
	}

	return result;
}

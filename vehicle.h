#ifndef VEHICLE_H
#define VEHICLE_H

#include "medlay/ctvec2.h"
#include "medlay/ctobject.h"

#include <iostream>
#include <string>

//#define PI 					3.14159265
#define HOVER_ACC			0.3f

#define VEHICLE_SIZE_X			64
#define VEHICLE_SIZE_Y			64
#define VEHICLE_HALFSIZE_X		32
#define VEHICLE_HALFSIZE_Y		32

enum base_type
{
	base_hover = 0,
	base_tank,
	base_car,
};

struct VEHICLE_CONTROLS
{
	float	base_forward;
	float	base_turn;

	VEHICLE_CONTROLS()
	{
		base_forward = 0;
		base_turn = 0;
	}
};

struct vehicle
{
	bool		m_active;
	int			m_type_base;
	ctobject	m_so_body;
	bool		m_collision;

	vehicle();
};

void vehicle_initialize(vehicle* a, int base, CTVEC2f pos, double angle);
void vehicle_update(vehicle* a);
void vehicle_reset(vehicle* a, CTVEC2f pos, double angle);
void vehicle_movement(vehicle* a, VEHICLE_CONTROLS& c);

#endif  // VEHICLE_H

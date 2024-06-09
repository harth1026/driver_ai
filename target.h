#ifndef TARGET_H
#define TARGET_H

#include "medlay/ctvec2.h"
#include "medlay/ctobject.h"

#include <iostream>
#include <string>

#define TARGET_SIZE_X			64
#define TARGET_SIZE_Y			64
#define TARGET_HALFSIZE_X		32
#define TARGET_HALFSIZE_Y		32

struct target
{
	bool		m_active;
	ctobject	m_so_body;
	bool		m_collision;

	target();
};

void target_initialize(target* a, CTVEC2f pos);
void target_update(target* a);
void target_reset(target* a, CTVEC2f pos);

#endif  // TARGET_H

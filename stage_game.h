#ifndef STAGE_GAME_H
#define STAGE_GAME_H

#include "medlay/ctmedlay.h"
#include "senn/senn.h"

#include <string>
#include <vector>

#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "stage.h"
#include "vehicle.h"
#include "target.h"

#define BOARD_SIZEX				800
#define BOARD_SIZEY				600

#define TOTAL_PARALLEL_GAMES	100
#define BASESIZE				10

enum class GAMESTAGE
{
	START = 0,
	MAIN,
	PAUSE,
	WINNER,
};

class game
{
private:
	GAMESTAGE	m_stage;
	bool		m_quit;
	vehicle		m_vehicle[TOTAL_PARALLEL_GAMES];
	target		m_target;

	HENN		m_enn;

	std::string	m_filebase;

	int			m_generation;
	int			m_bestmaximum;
	int			m_bestaverage;
	int			m_bestgeneration;

	// KEY HANDLES
	bool		m_keyesc;
	bool		m_keyr;

	// TIMING
	clock_t		m_starttime;
	clock_t		m_starttarget;

	// images
	MEDLAY_IMAGE	img_base;
	MEDLAY_IMAGE	img_target;

public:
	game();
	~game();

	bool initialize();
	bool uninitialize();
	bool reset();
	STAGE update(int timepoint);
	bool handle_keyup(EVENTKEY keycode);
	bool handle_keydown(EVENTKEY keycode); 
	bool handle_mousemove(CTVEC2 mouse);
	bool handle_mousebuttondown(CTVEC2 mouse);
	bool draw();
};

#endif // GAME_H

#include "stage_game.h"
#include <iostream>

#include "medlay/ctmath.h"

#define ROUND_LENGTH			15	// seconds
#define TARGET_RESET_LENGTH		5	// seconds

#define OUTPUT_TOLERANCE		0.1

#define DRIVER_BASE				base_hover
//#define DRIVER_BASE				base_tank
//#define DRIVER_BASE			base_car		// TODO


#define VEHICLE_START			CTVEC2f(BOARD_SIZEX / 2, BOARD_SIZEY / 2)

#define USE_SAVEFILE			1

#define POINTS_IDLE				-10
#define POINTS_OUTOFBOUNDS		-50
#define POINTS_TIME				1
#define POINTS_COLLECTTARGET	20
#define POINTS_ONTARGET			2


#define initimage(imgtag,imgfile);\
	{ CTCOLOR transparent = CTCOLOR(255, 0, 255);\
	if (imgtag) { ctmedlay_freeimage(imgtag); imgtag = nullptr; }\
	ctmedlay_addimage_v11(imgfile, &imgtag, &transparent); }

game::game()
{
	m_stage = GAMESTAGE::START;
	m_quit = false;
	m_keyr = false;
	m_keyesc = false;

	m_starttime = 0;

	img_base = nullptr;
	img_target = nullptr; 

	m_generation = 0;
	m_bestmaximum = 0;
	m_bestaverage = 0;
	m_bestgeneration = 0;
}

game::~game()
{
}

bool game::initialize()
{
	m_stage = GAMESTAGE::START;
	m_quit = false;
	m_keyesc = false;

	for (int i = 0; i < TOTAL_PARALLEL_GAMES; i++)
	{
		vehicle_initialize(&(m_vehicle[i]), DRIVER_BASE, VEHICLE_START, 0);
	}
	CTVEC2f target_start = CTVEC2f((float)(rand() % BOARD_SIZEX), (float)(rand() % BOARD_SIZEY));
	target_initialize(&(m_target), target_start);

#if USE_SAVEFILE
	m_filebase = std::string("driver_") + std::to_string(int(DRIVER_BASE));
#endif

	switch (m_vehicle[0].m_type_base)
	{
	case base_tank:			initimage(img_base, "pics/base_tank.bmp");		break;
	case base_hover:		initimage(img_base, "pics/base_hover.bmp");		break;
	default:				initimage(img_base, "pics/base_tank.bmp");		break;
	}

	initimage(img_target, "pics/target.bmp");

	m_generation = 0;
	m_bestmaximum = 0;
	m_bestaverage = 0;

	/* TOTAL AI
	BASE DEFAULT
	* INPUTS - 5
		POSITION SELF 2x
		RELATIVE POSITION TARGET 2x
		ANGLE BODY SELF
	* OUTPUTS - 2
		TURN BODY
		MOVE BODY

	HOVER
	* INPUTS - 6
		POSITION SELF 2x
		VELOCITY SELF 2x
 		RELATIVE POSITION TARGET 2x
	* OUTPUTS - 2
		MOVE LEFT / RIGHT
		MOVE UP / DOWN

	*/

	int inputs;
	int outputs;

	switch (DRIVER_BASE)
	{
	case base_tank:
		inputs = 5;
		outputs = 2;
		break;

	case base_hover:
		inputs = 6;
		outputs = 2;
		break;

	default:
		inputs = 5;
		outputs = 2;
		break;
	}

	senn_initialize(&m_enn);
	senn_setpropertyvalue(m_enn, CTPROP::PID_INPUTS, inputs);
	senn_setpropertyvalue(m_enn, CTPROP::PID_OUTPUTS, outputs);
	senn_setpropertyvalue(m_enn, CTPROP::PID_BASESIZE, BASESIZE);
	senn_setpropertyvalue(m_enn, CTPROP::PID_CLUSTERSIZE, TOTAL_PARALLEL_GAMES);
	senn_initializebase(m_enn);
#if USE_SAVEFILE
	for (int i = 0; i < BASESIZE; i++)
	{
		std::string filename = m_filebase + std::to_string(i) + std::string(".txt");
		senn_loadbasenetworkfromfile(m_enn, i, filename);
	}
#endif
	senn_createclusterfrombase(m_enn);

	m_starttarget = m_starttime = clock();
	return true;
}

bool game::uninitialize()
{
#if USE_SAVEFILE
	std::cout << "saving" << std::endl;
	std::string filename;
	for (int i = 0; i < BASESIZE; i++)
	{
		filename = m_filebase + std::to_string(i) + std::string(".txt");
		senn_savebasenetworktofile(m_enn, i, filename);
	}
#endif
	ctmedlay_freeallimages();
	return true;
}

bool game::reset()
{
	m_generation++;

	// find best driver
	double maxFitness = -999999.0f;
	double ttlFitness = 0;
	double avgFitness = 0;
	int bestindex = -1;

	for (int i = 0; i < TOTAL_PARALLEL_GAMES; i++)
	{
		float f = 0;

		senn_getfitness(m_enn, i, &f);
		ttlFitness += f;
		if (maxFitness < f)
		{
			maxFitness = f;
			bestindex = i;
		}
	}

	avgFitness = ttlFitness / TOTAL_PARALLEL_GAMES;

	if (m_bestmaximum < (int)maxFitness)
		m_bestmaximum = (int)maxFitness;

	if (m_bestaverage < (int)avgFitness)
	{
		m_bestaverage = (int)avgFitness;
		m_bestgeneration = m_generation;
	}

	std::cout << "\n\nCURRENT GENERATION: " << m_generation << std::endl;
	senn_printdebug(m_enn, bestindex);

	std::cout << "BEST INDEX:      " << bestindex << std::endl;
	std::cout << "BEST FITNESS:    " << maxFitness << std::endl;
	std::cout << "AVERAGE FITNESS: " << avgFitness << std::endl;

	senn_selectbasenetworks(m_enn);
	senn_createclusterfrombase(m_enn);


	for (int i = 0; i < TOTAL_PARALLEL_GAMES; i++)
	{
		vehicle_reset(&(m_vehicle[i]), VEHICLE_START, 0);
		senn_setfitness(m_enn, i, 0);
	}

	CTVEC2f nexttargetpos = CTVEC2f((float)(rand() % BOARD_SIZEX), (float)(rand() % BOARD_SIZEY));
	target_reset(&(m_target), nexttargetpos);

	m_starttarget = m_starttime = clock();
	return true;
}

STAGE game::update(int timepoint)
{
	bool stillplaying = false;

	for (int i = 0; i < TOTAL_PARALLEL_GAMES; i++)
	{
		if (m_vehicle[i].m_active)
		{
			stillplaying = true;

			vehicle_update(&(m_vehicle[i]));
			senn_addfitness(m_enn, i,  POINTS_TIME);

			// detect out of bounds
			bool outofbounds = false;
			if (m_vehicle[i].m_so_body.m_pos.cx < 0)			outofbounds = true;
			if (m_vehicle[i].m_so_body.m_pos.cy < 0)			outofbounds = true;
			if (m_vehicle[i].m_so_body.m_pos.cx > BOARD_SIZEX)	outofbounds = true;
			if (m_vehicle[i].m_so_body.m_pos.cy > BOARD_SIZEY)	outofbounds = true;

			if (outofbounds)
			{
				m_vehicle[i].m_active = false;
				senn_addfitness(m_enn, i, POINTS_OUTOFBOUNDS);
			}

			// detect collision
			float distance = 0;
			distance = ctmath_distance(m_vehicle[i].m_so_body.m_pos, m_target.m_so_body.m_pos);
			if (distance < 64.0f)
			{
				senn_addfitness(m_enn, i, POINTS_ONTARGET);
			}

			VEHICLE_CONTROLS tc;

			/* TOTAL AI
			BASE DEFAULT
			* INPUTS - 5
				POSITION SELF 2x
				RELATIVE POSITION TARGET 2x
				ANGLE BODY SELF
			* OUTPUTS - 2
				TURN BODY
				MOVE BODY

			HOVER
			* INPUTS - 6
				POSITION SELF 2x
				VELOCITY SELF 2x
				RELATIVE POSITION TARGET 2x
			* OUTPUTS - 2
				MOVE LEFT / RIGHT
				MOVE UP / DOWN

			*/

			std::vector<double> inputs;
			std::vector<double> outputs;
			double dval;
			double r_ang = 0;
			double d_ang = 0;

			inputs.clear();
			outputs.clear();
			tc.base_forward = 0.0f;
			tc.base_turn = 0.0f;

			switch (DRIVER_BASE)
			{

			case base_tank:
			default:
				/*
					POSITION SELF 2x
					RELATIVE POSITION TO TARGET 2x
					RELATIVE ANGLE BODY SELF
				*/
				// POSITION SELF 2x
				dval = (double)m_vehicle[i].m_so_body.m_pos.cx / BOARD_SIZEX;
				inputs.push_back(dval);
				dval = (double)m_vehicle[i].m_so_body.m_pos.cy / BOARD_SIZEY;
				inputs.push_back(dval);

				// 	RELATIVE POSITION TO TARGET 2x
				dval = (double)(m_target.m_so_body.m_pos.cx - m_vehicle[i].m_so_body.m_pos.cx) / BOARD_SIZEX;
				inputs.push_back(dval);
				dval = (double)(m_target.m_so_body.m_pos.cy - m_vehicle[i].m_so_body.m_pos.cy) / BOARD_SIZEY;
				inputs.push_back(dval);

				// RELATIVE ANGLE BODY SELF
				dval = ctmath_relativeangle(m_vehicle[i].m_so_body.m_pos, m_target.m_so_body.m_pos, m_vehicle[i].m_so_body.m_ang);
				inputs.push_back(dval);
				break;

			case base_hover:
				// POSITION SELF 2x
				dval = (double)m_vehicle[i].m_so_body.m_pos.cx / BOARD_SIZEX;
				inputs.push_back(dval);
				dval = (double)m_vehicle[i].m_so_body.m_pos.cy / BOARD_SIZEY;
				inputs.push_back(dval);

				// VELOCITY SELF 2x
				dval = (double)m_vehicle[i].m_so_body.m_vel.cx;
				inputs.push_back(dval);
				dval = (double)m_vehicle[i].m_so_body.m_vel.cy;
				inputs.push_back(dval);

				// 	RELATIVE POSITION TO TARGET 2x
				dval = (double)(m_target.m_so_body.m_pos.cx - m_vehicle[i].m_so_body.m_pos.cx) / BOARD_SIZEX;
				inputs.push_back(dval);
				dval = (double)(m_target.m_so_body.m_pos.cy - m_vehicle[i].m_so_body.m_pos.cy) / BOARD_SIZEY;
				inputs.push_back(dval);
				break;
			}
			/*
			DEFAULT
				0 - TURN BODY
				1 - MOVE BODY
			HOVER
				0 - LEFT / RIGHT
				1 - UP / DOWN
			*/

			senn_feedforward(m_enn, i, inputs, outputs);
			tc.base_turn	= (float)outputs[0];
			tc.base_forward = (float)outputs[1];

			inputs.clear();
			outputs.clear();

			vehicle_movement(&m_vehicle[i], tc);
		}
	}

	if (m_keyesc)
	{
		return STAGE::QUIT;
	}

	clock_t currenttime = clock();
	int targettime = (currenttime - m_starttarget) / CLOCKS_PER_SEC;

	int maxdistance = (int)sqrt(BOARD_SIZEX * BOARD_SIZEX + BOARD_SIZEY * BOARD_SIZEY);

	if (targettime > TARGET_RESET_LENGTH)
	{
		// add points for how close to target when it resets
		for (int i = 0; i < TOTAL_PARALLEL_GAMES; i++)
		{
			if (m_vehicle[i].m_active)
			{
				int distance = (int)ctmath_distance(m_vehicle[i].m_so_body.m_pos, m_target.m_so_body.m_pos);
				senn_addfitness(m_enn, i, maxdistance - distance);
			}
		}

		m_starttarget = currenttime;
		CTVEC2f nexttargetpos = CTVEC2f((float)(rand() % BOARD_SIZEX), (float)(rand() % BOARD_SIZEY));
		target_reset(&(m_target), nexttargetpos);
	}

	int roundtime = (currenttime - m_starttime) / CLOCKS_PER_SEC;
	if(roundtime > ROUND_LENGTH)
		stillplaying = false;

	if(!stillplaying)
		reset();

	return STAGE::GAME;
}

bool game::handle_keyup(EVENTKEY keycode)
{
	switch (keycode)
	{
	case EVENTKEY::K_ESCAPE:		m_keyesc = false;		break;
	case EVENTKEY::K_r:				m_keyr = false;			break;
	default:	break;
	}
	return true;
}

bool game::handle_keydown(EVENTKEY keycode) 
{
	switch (keycode)
	{
	case EVENTKEY::K_ESCAPE:		m_keyesc = true;		break;
	case EVENTKEY::K_r:				m_keyr = true; 			break;
	default:	break;
	}
	return true;
}

bool game::handle_mousemove(CTVEC2 mouse)
{
	return true;
}

bool game::handle_mousebuttondown(CTVEC2 mouse)
{
	return true;
}

void draw_vehicle(vehicle* t, MEDLAY_IMAGE img_base)
{
	if (t)
	{
		CTVEC2 pos = CTVEC2(int(t->m_so_body.m_pos.cx - 32), int(t->m_so_body.m_pos.cy - 32));
		ctmedlay_setimagetodisplay_adv(img_base, pos, t->m_so_body.m_ang);
	}
}

void draw_target(target* t, MEDLAY_IMAGE img_target)
{
	if (t)
	{
		CTVEC2 pos = CTVEC2(int(t->m_so_body.m_pos.cx - 32), int(t->m_so_body.m_pos.cy - 32));
		ctmedlay_setimagetodisplay_adv(img_target, pos, 0);
	}
}

bool game::draw()
{
	for (int i = 0; i < TOTAL_PARALLEL_GAMES; i++)
	{
		if (m_vehicle[i].m_active)
			draw_vehicle(&m_vehicle[i], img_base);
	}
	draw_target(&m_target, img_target);

	return true;
}


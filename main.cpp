
#include <time.h>

#include "stage.h"
#include "stage_game.h"

#define GAME_NAME		"DRIVER AI"

bool g_quit = false;

game mygame;

bool stage_init(STAGE stage)
{
	bool result = false;

	switch (stage)
	{
	case STAGE::MENU:
		break;
	case STAGE::GAME:
		result = mygame.initialize();
		break;
	default:	break;
	}

	return result;
}

bool stage_exit(STAGE stage)
{
	bool result = false;

	switch (stage)
	{
	case STAGE::MENU:
		break;
	case STAGE::GAME:
		result = mygame.uninitialize();
		break;
	default:	break;
	}

	return result;
}

STAGE stage_update(STAGE stage, int ticks)
{
	STAGE result = STAGE::QUIT;

	switch (stage)
	{
	case STAGE::MENU:
		break;
	case STAGE::GAME:
		result = mygame.update(ticks);
		break;
	default:	break;
	}

	return result;
}

bool stage_draw(STAGE stage)
{
	bool result = false;

	ctmedlay_cleardisplay();

	switch (stage)
	{
	case STAGE::MENU:
		break;
	case STAGE::GAME:
		result = mygame.draw();
		break;
	default:	break;
	}

	if (result)
		ctmedlay_displayimages();
	return result;
}

bool stage_keyup(STAGE stage)
{
	bool result = false;

	EVENTKEY keycode;
	ctmedlay_geteventkey(&keycode);

	switch (stage)
	{
	case STAGE::MENU:
		break;
	case STAGE::GAME:
		result = mygame.handle_keyup(keycode);
		break;
	default:	break;
	}

	return result;
}

bool stage_keydown(STAGE stage)
{
	bool result = false;

	EVENTKEY keycode;
	ctmedlay_geteventkey(&keycode);

	switch (stage)
	{
	case STAGE::MENU:
		break;
	case STAGE::GAME:
		result = mygame.handle_keydown(keycode);
		break;
	default:	break;
	}

	return result;
}

bool initialize()
{
	return ctmedlay_initialize(GAME_NAME, BOARD_SIZEX, BOARD_SIZEY);
}

bool uninitialize()
{
	return ctmedlay_uninitialize();
}

int main(int argc, char* args[])
{
	srand((unsigned int)time(nullptr));
	STAGE stage = STAGE::MENU;
	STAGE stage_next = STAGE::MENU;

	g_quit = false;

	int time_current = 0;
	int time_next = 0;

	if (initialize())
	{
		stage = STAGE::GAME;
		stage_next = STAGE::GAME;

		if (stage_init(stage))
		{
			EVENTTYPE et;
			while (!g_quit)
			{
				while (ctmedlay_pollevent(&et))
				{
					switch (et)
					{
					case EVENTTYPE::E_KEYUP:
						stage_keyup(stage);
						break;
					case EVENTTYPE::E_KEYDOWN:
						stage_keydown(stage);
						break;
					default:	break;
					}
				}

				time_current = ctmedlay_getticks();
				if (time_current >= time_next)
				{
					time_next = time_current + 30;
					stage_next = stage_update(stage, ctmedlay_getticks());
					stage_draw(stage);

					if (stage != stage_next)
					{
						stage_exit(stage);
						if (stage_next == STAGE::QUIT)
						{
							g_quit = true;
						}
						else if (stage_init(stage_next))
						{
							stage = stage_next;
						}
					}
				}
			}
		}
		uninitialize();
	}

	return 0;
}



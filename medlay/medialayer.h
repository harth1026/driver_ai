#ifndef MEDIALAYER_H
#define MEDIALAYER_H

#include <vector>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h> 
#include <SDL2/SDL_ttf.h>

#include "ctmedlay.h"

#define			USE_AUDIO		1

#ifdef WIN32
#include <time.h>
#elif LINUX
#include <sys/time.h>
#endif


class medialayer
{
private:

	std::vector<MEDLAY_IMAGE>	m_imagelist;
	std::vector<MEDLAY_MUSIC>	m_musiclist;
	std::vector<MEDLAY_SOUND>	m_soundlist;
	std::vector<MEDLAY_FONT>	m_fontlist;

	SDL_Window* m_window;
	SDL_Surface* m_display;
	SDL_Renderer* m_renderer;
	SDL_Keycode				m_key;

	SDL_Texture* loadTexture(std::string path, bool transparentbackground);

	int m_screen_x;
	int m_screen_y;

public:
	medialayer();
	~medialayer();

	// management
	bool initialize(std::string windowname, int screenx, int screeny);
	bool uninitialize();
	bool getscreensize(int* size_x, int* size_y);
	bool pollevent(EVENTTYPE* eventtype);
	bool geteventkey(EVENTKEY* eventkey);
	bool getmouseposition(int* pos_x, int* pos_y);
	int  getticks();
	bool sleep(int milliseconds);

	// graphics
		// primitives
	bool drawline(int ax, int ay, int bx, int by, int r, int g, int b);

	// bitmaps
	bool addimage(std::string bmpfile, MEDLAY_IMAGE* imageref, bool transparentbackground);
	bool addimage_v11(std::string bmpfile, MEDLAY_IMAGE* imageref, CTCOLOR* transparentcolor);
	bool arraytoimage(uint32_t* pixels, MEDLAY_IMAGE* imageref, int pitch);

	bool freeallimages();
	bool freeimage(MEDLAY_IMAGE imageref);
	bool setimagetodisplay(MEDLAY_IMAGE imageref, int pos_x, int pos_y, CRECT* srcrect = nullptr);
	bool setimagetodisplay_adv(MEDLAY_IMAGE imageref, int pos_x, int pos_y, double angle);
	bool getimagesize(MEDLAY_IMAGE imageref, int* size_x, int* size_y);

	bool setalphamodulation(MEDLAY_IMAGE imageref, int value);

	// text
	bool addfont(std::string fontfile, int fontsize, MEDLAY_FONT* fontref);
	bool freeallfonts();
	bool freefont(MEDLAY_FONT fontref);
	bool texttoimage(MEDLAY_IMAGE* imageref, MEDLAY_FONT fontref, std::string text, CTCOLOR col);

	// general
	bool cleardisplay();
	bool displayimages();


	// audio
		// music
	bool addmusic(std::string wavfile, MEDLAY_MUSIC* musicref);
	bool freeallmusic();
	bool freemusic(MEDLAY_MUSIC musicref);
	bool playmusic(MEDLAY_MUSIC musicref);
	bool stopmusic();
	bool pausemusic();
	bool resumemusic();
	bool setmusicvolume(int volume);

	// sound
	bool addsound(std::string wavfile, MEDLAY_SOUND* soundref);
	bool freeallsound();
	bool freesound(MEDLAY_SOUND soundref);
	bool playsound(MEDLAY_SOUND soundref);
	bool setsoundvolume(int volume, int channel);
};

#endif
 
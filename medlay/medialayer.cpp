#include <math.h>
#include <string>
#include <memory>

#include "medialayer.h"

SDL_Texture* medialayer::loadTexture(std::string path, bool transparentbackground)
{
	SDL_Texture* outputTexture = nullptr;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface)
	{
		if (transparentbackground)
		{
			SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 128, 255));
		}

		//Convert surface to texture
		outputTexture = SDL_CreateTextureFromSurface(m_renderer, loadedSurface);
		SDL_FreeSurface(loadedSurface);
	}

	return outputTexture;
}


medialayer::medialayer()
{
	m_window = nullptr;
	m_display = nullptr;
	m_renderer = nullptr;
	m_screen_x = 0;
	m_screen_y = 0;
	m_key = SDLK_0;

	m_imagelist.clear();

}

medialayer::~medialayer()
{
}

bool medialayer::initialize(std::string windowname, int screenx, int screeny)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		return false;

	m_window = SDL_CreateWindow(windowname.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenx, screeny, SDL_WINDOW_SHOWN);
	if (m_window == nullptr)
		return false;

	m_screen_x = screenx;
	m_screen_y = screeny;

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr)
		return false;

	SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	int flags = IMG_INIT_JPG;
	int myresult = IMG_Init(flags);

	if (myresult == 0)
	{
		return false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		return false;

	if (TTF_Init() == -1)
		return false;

	return true;
}

bool medialayer::uninitialize()
{
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	m_renderer = nullptr;
	m_window = nullptr;

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();

	return true;
}

bool medialayer::getscreensize(int* size_x, int* size_y)
{
	bool result = false;
	if (size_x && size_y)
	{
		*size_x = m_screen_x;
		*size_y = m_screen_y;
		result = true;
	}

	return result;
}

bool medialayer::pollevent(EVENTTYPE* eventtype)
{
	bool result = false;
	if (eventtype)
	{
		SDL_Event e;
		if (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
				*eventtype = EVENTTYPE::E_QUIT;
				result = true;
				break;
			case SDL_KEYDOWN:
				*eventtype = EVENTTYPE::E_KEYDOWN;
				m_key = e.key.keysym.sym;
				result = true;
				break;
			case SDL_KEYUP:
				*eventtype = EVENTTYPE::E_KEYUP;
				m_key = e.key.keysym.sym;
				result = true;
				break;
			case SDL_MOUSEMOTION:
				*eventtype = EVENTTYPE::E_MOUSEMOTION;
				result = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				*eventtype = EVENTTYPE::E_MOUSEBUTTONDOWN;
				result = true;
				break;
			case SDL_MOUSEBUTTONUP:
				*eventtype = EVENTTYPE::E_MOUSEBUTTONUP;
				result = true;
				break;
			default:			// unsupported SDL_Event.type
				break;
			}
		}
	}
	return result;
}

bool medialayer::geteventkey(EVENTKEY* eventkey)
{
	bool result = false;
	if (eventkey)
	{
		*eventkey = (EVENTKEY)m_key;
		result = true;
	}

	return result;
}

bool medialayer::getmouseposition(int* pos_x, int* pos_y)
{
	bool result = false;

	if (pos_x && pos_y)
	{
		SDL_GetMouseState(pos_x, pos_y);
		result = true;
	}

	return result;
}

int medialayer::getticks()
{
	int value = 0;
	value = (int)SDL_GetTicks();
	return value;
}

bool medialayer::sleep(int milliseconds)
{
	SDL_Delay(milliseconds);
	return true;
}


bool medialayer::drawline(int ax, int ay, int bx, int by, int r, int g, int b)
{
	SDL_SetRenderDrawColor(m_renderer, r, g, b, 0xFF);
	SDL_RenderDrawLine(m_renderer, ax, ay, bx, by);
	return true;
}

bool medialayer::addimage(std::string bmpfile, MEDLAY_IMAGE* imageref, bool transparentbackground)
{
	bool result = false;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(bmpfile.c_str());
	if (loadedSurface)
	{
		if (transparentbackground)
		{
			SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 128, 255));
		}

		//Convert surface to texture
		*imageref = (MEDLAY_IMAGE)SDL_CreateTextureFromSurface(m_renderer, loadedSurface);
		SDL_FreeSurface(loadedSurface);
	}

	if (*imageref)
	{
		m_imagelist.push_back(*imageref);
		result = true;
	}

	return result;
}

bool medialayer::addimage_v11(std::string bmpfile, MEDLAY_IMAGE* imageref, CTCOLOR* transparentcolor)
{
	bool result = false;

	//Load image at specified path
	std::unique_ptr<SDL_Surface> loadedSurface(IMG_Load(bmpfile.c_str()));

	if (loadedSurface)
	{
		if (transparentcolor != nullptr)
		{
			SDL_SetColorKey(loadedSurface.get(), SDL_TRUE, SDL_MapRGB(loadedSurface->format, transparentcolor->r, transparentcolor->g, transparentcolor->b));
		}

		//Convert surface to texture
		*imageref = (MEDLAY_IMAGE)SDL_CreateTextureFromSurface(m_renderer, loadedSurface.get());
		SDL_FreeSurface(loadedSurface.get());
		loadedSurface.release();
	}

	if (*imageref)
	{
		m_imagelist.push_back(*imageref);
		result = true;
	}

	return result;
}

bool medialayer::arraytoimage(uint32_t* pixels, MEDLAY_IMAGE* imageref, int pitch)
{
	bool result = false;

	if (SDL_UpdateTexture((SDL_Texture*)*imageref, NULL, pixels, pitch) == 0)
		result = true;

	return result;
}


bool medialayer::freeallimages()
{
	for (size_t i = 0; i < m_imagelist.size(); i++)
	{
		SDL_DestroyTexture((SDL_Texture*)m_imagelist[i]);
	}
	m_imagelist.clear();
	return true;
}

bool medialayer::freeimage(MEDLAY_IMAGE imageref)
{
	bool result = false;
	for (size_t i = 0; i < m_imagelist.size(); i++)
	{
		if (m_imagelist[i] == (SDL_Texture*)imageref)
		{
			SDL_DestroyTexture((SDL_Texture*)m_imagelist[i]);
			m_imagelist.erase(m_imagelist.begin() + i);
			result = true;
		}
	}

	return result;
}

bool medialayer::setimagetodisplay(MEDLAY_IMAGE imageref, int pos_x, int pos_y, CRECT* srcrect)
{
	bool result = false;
	if (imageref)
	{
		SDL_Rect* srcpos = nullptr;
		int w = 0;
		int h = 0;
		SDL_QueryTexture((SDL_Texture*)imageref, NULL, NULL, &w, &h);

		SDL_Rect dstpos = SDL_Rect();
		dstpos.w = w;
		dstpos.h = h;
		dstpos.x = pos_x;
		dstpos.y = pos_y;

		if (srcrect)
		{
			srcpos = new SDL_Rect();
			srcpos->x = srcrect->x;
			srcpos->y = srcrect->y;
			srcpos->w = srcrect->w;
			srcpos->h = srcrect->h;
			dstpos.w = srcrect->w;
			dstpos.h = srcrect->h;
		}

		SDL_RenderCopy(m_renderer, (SDL_Texture*)imageref, srcpos, &dstpos);
		result = true;
	}

	return result;
}

bool medialayer::setimagetodisplay_adv(MEDLAY_IMAGE imageref, int pos_x, int pos_y, double angle)
{
	bool result = false;
	if (imageref)
	{
		SDL_Rect dstpos;
		SDL_QueryTexture((SDL_Texture*)imageref, NULL, NULL, &dstpos.w, &dstpos.h);

		dstpos.x = pos_x;
		dstpos.y = pos_y;

		SDL_RenderCopyEx(m_renderer, (SDL_Texture*)imageref, nullptr, &dstpos, angle, nullptr, SDL_FLIP_NONE);
		result = true;
	}

	return result;
}

bool medialayer::getimagesize(MEDLAY_IMAGE imageref, int* size_x, int* size_y)
{
	bool result = false;
	if (size_x && size_y)
	{
		SDL_QueryTexture((SDL_Texture*)imageref, nullptr, nullptr, size_x, size_y);
		result = true;
	}
	return result;
}

bool medialayer::setalphamodulation(MEDLAY_IMAGE imageref, int value)
{
	SDL_SetTextureAlphaMod((SDL_Texture*)imageref, value);
	return true;
}

bool medialayer::addfont(std::string fontfile, int fontsize, MEDLAY_FONT* fontref)
{
	bool result = false;

	* fontref = (MEDLAY_FONT)TTF_OpenFont(fontfile.c_str(), fontsize);
	if (*fontref)
	{
		m_fontlist.push_back((TTF_Font*)*fontref);
		result = true;
	}
	return result;
}

bool medialayer::freeallfonts()
{
	for (size_t i = 0; i < m_fontlist.size(); i++)
	{
		TTF_CloseFont((TTF_Font*)m_fontlist[i]);
	}
	m_fontlist.clear();
	return true;
}

bool medialayer::freefont(MEDLAY_FONT fontref)
{
	bool result = false;
	for (size_t i = 0; i < m_fontlist.size(); i++)
	{
		if (m_fontlist[i] == (TTF_Font*)fontref)
		{
			TTF_CloseFont((TTF_Font*)m_fontlist[i]);
			m_fontlist.erase(m_fontlist.begin() + i);
			result = true;
		}
	}

	return result;
}

bool medialayer::texttoimage(MEDLAY_IMAGE* imageref, MEDLAY_FONT fontref, std::string text, CTCOLOR col)
{
	bool result = false;
	SDL_Color color;
	color.r = col.r;
	color.g = col.g;
	color.b = col.b;

	SDL_Surface* tempsurface = TTF_RenderText_Solid((TTF_Font*)fontref, text.c_str(), color);
	if (tempsurface)
	{
		*imageref = (MEDLAY_IMAGE)SDL_CreateTextureFromSurface(m_renderer, tempsurface);
		SDL_FreeSurface(tempsurface);
		if (*imageref)
		{
			m_imagelist.push_back(*imageref);
			result = true;
		}
	}
	return result;
}


bool medialayer::cleardisplay()
{
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(m_renderer);

	return true;
}

bool medialayer::displayimages()
{
	SDL_RenderPresent(m_renderer);

	return true;
}

bool medialayer::addmusic(std::string wavfile, MEDLAY_MUSIC* musicref)
{
	bool result = false;

	* musicref = (MEDLAY_MUSIC)Mix_LoadMUS(wavfile.c_str());
	if (*musicref)
	{
		m_musiclist.push_back(*musicref);
		result = true;
	}
	return result;
}

bool medialayer::freeallmusic()
{
	for (size_t i = 0; i < m_musiclist.size(); i++)
	{
		Mix_FreeMusic((Mix_Music*)m_musiclist[i]);
	}
	m_musiclist.clear();
	return true;
}

bool medialayer::freemusic(MEDLAY_MUSIC musicref)
{
	bool result = false;
	for (size_t i = 0; i < m_musiclist.size(); i++)
	{
		if (m_musiclist[i] == (Mix_Music*)musicref)
		{
			Mix_FreeMusic((Mix_Music*)m_musiclist[i]);
			m_musiclist.erase(m_musiclist.begin() + i);
			result = true;
		}
	}

	return result;
}

bool medialayer::playmusic(MEDLAY_MUSIC musicref)
{
	bool result = false;
	if (Mix_PlayingMusic() == 0)
	{
		Mix_PlayMusic((Mix_Music*)musicref, -1);
		result = true;
	}
	return result;
}

bool medialayer::stopmusic()
{
	Mix_HaltMusic();
	return true;
}

bool medialayer::pausemusic()
{
	if (Mix_PausedMusic() == 0)
	{
		Mix_ResumeMusic();
	}
	return true;
}

bool medialayer::resumemusic()
{
	if (Mix_PausedMusic() == 1)
	{
		Mix_PauseMusic();
	}
	return true;
}

bool medialayer::setmusicvolume(int volume)
{
	Mix_VolumeMusic(volume);
	return true;
}

bool medialayer::addsound(std::string wavfile, MEDLAY_SOUND* soundref)
{
	bool result = false;

	* soundref = (MEDLAY_SOUND)Mix_LoadWAV(wavfile.c_str());
	if (*soundref)
	{
		m_soundlist.push_back(*soundref);
		result = true;
	}
	return result;
}

bool medialayer::freeallsound()
{
	for (size_t i = 0; i < m_soundlist.size(); i++)
	{
		Mix_FreeChunk((Mix_Chunk*)m_soundlist[i]);
	}
	m_soundlist.clear();
	return true;
}

bool medialayer::freesound(MEDLAY_SOUND soundref)
{
	bool result = false;
	for (size_t i = 0; i < m_soundlist.size(); i++)
	{
		if (m_soundlist[i] == (Mix_Chunk*)soundref)
		{
			Mix_FreeChunk((Mix_Chunk*)m_soundlist[i]);
			m_soundlist.erase(m_soundlist.begin() + i);
			result = true;
		}
	}

	return result;
}

bool medialayer::playsound(MEDLAY_SOUND soundref)
{
	bool result = false;
	if (soundref)
	{
		Mix_PlayChannel(-1, (Mix_Chunk*)soundref, 0);
		result = true;
	}

	return result;
}

bool medialayer::setsoundvolume(int volume, int channel)
{
	Mix_Volume(channel, volume);
	return true;
}



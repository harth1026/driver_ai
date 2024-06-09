
#ifdef WIN32
#include <Windows.h>
#endif

#include <string>

#include "ctmedlay.h"
#include "medialayer.h"
#include "../utilities/pglogger.h"

#if WIN32

HKEY OpenKey(HKEY hRootKey, const char* stringKey)
{
	HKEY hKey;

	wchar_t wtext[64];
	size_t retval = 64;
	mbstowcs_s(&retval, wtext, stringKey, strlen(stringKey) + 1);
	LPWSTR ptr = wtext;

	// TODO: MODIFY TO AVOID ERROR
	// IF ERROR, USE UNICODE CHARACTER SET
	RegOpenKeyEx(hRootKey, ptr, NULL, KEY_READ, &hKey);
	return hKey;
}

// Set the variable to some data
void SetValue(HKEY hKey, LPCTSTR lpValue, DWORD data)
{
	RegSetValueEx(hKey, lpValue, NULL, REG_DWORD, (LPBYTE)&data, sizeof(DWORD));
}

DWORD GetDWORDValue(HKEY hKey, LPCTSTR lpValue)
{
	DWORD data;
	DWORD size = sizeof(data);
	DWORD type = REG_DWORD;
	RegQueryValueEx(hKey, lpValue, NULL, &type, (LPBYTE)&data, &size);

	return data;
}

#endif



static medialayer* ml = nullptr;

pglogger			g_debuglog;
char				g_logtext[256];

bool ctmedlay_initialize(std::string windowname, int screenx, int screeny)
{
	bool result = false;
	int logenable = 0;
#ifdef WIN32
	HKEY hKey = OpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Crossturn\\libctmedlay");
	logenable = (int)GetDWORDValue(hKey, L"apilog.enable");

#endif
	if (logenable > 0)
	{
		g_debuglog.enablelog("c:\\temp\\ctmedlaylog.txt", LEVEL_BASIC);

		snprintf(g_logtext, 256, "%s() -> windowname=%s screen=%dx%d", __func__, windowname.c_str(), screenx, screeny);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml == nullptr)
	{
		ml = new medialayer;

		if (ml)
		{
			result = ml->initialize(windowname, screenx, screeny);
		}
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_uninitialize()
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->uninitialize();
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_getscreensize(CTVEC2* size)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		int cx = 0;
		int cy = 0;
		result = ml->getscreensize(&cx, &cy);
		if (result)
			*size = CTVEC2(cx, cy);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	return result;
}

bool ctmedlay_pollevent(EVENTTYPE* eventtype)
{
	bool result = false;

	//	if (g_debuglog.isactive())
	//	{
	//		snprintf(g_logtext, 256, "%s() ->", __func__);
	//		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	//	}

	if (ml)
	{
		result = ml->pollevent(eventtype);
	}

	//	if (g_debuglog.isactive())
	//	{
	//		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
	//		else		snprintf(g_logtext, 256, "%s() NG", __func__);
	//		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	//	}

	return result;
}

bool ctmedlay_geteventkey(EVENTKEY* eventkey)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->geteventkey(eventkey);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_getmouseposition(CTVEC2* pos)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		int cx = 0;
		int cy = 0;
		result = ml->getmouseposition(&cx, &cy);
		if (result)
			*pos = CTVEC2(cx, cy);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

int ctmedlay_getticks()
{
	int ms = -1;

	if (ml)
	{
		ms = ml->getticks();
	}

	return ms;
}

bool ctmedlay_sleep(int milliseconds)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->sleep(milliseconds);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_drawline(CTVEC2 a, CTVEC2 b, CTCOLOR col)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->drawline(a.cx, a.cy, b.cx, b.cy, col.r, col.g, col.b);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}


bool ctmedlay_addimage(std::string bmpfile, MEDLAY_IMAGE* imageref, bool transparentbackground)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		void* imgref = nullptr;
		result = ml->addimage(bmpfile, (MEDLAY_IMAGE*)&imgref, transparentbackground);
		if (result)
			*imageref = imgref;

	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_addimage_v11(std::string bmpfile, MEDLAY_IMAGE* imageref, CTCOLOR* transparentcolor)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		void* imgref = nullptr;
		result = ml->addimage_v11(bmpfile, (MEDLAY_IMAGE*)&imgref, transparentcolor);
		if (result)
			*imageref = imgref;

	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_arraytoimage(uint32_t* pixels, MEDLAY_IMAGE* imageref, int pitch)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		void* imgref = nullptr;
		result = ml->arraytoimage(pixels, (MEDLAY_IMAGE*)&imgref, pitch);
		if (result)
			*imageref = imgref;

	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_freeallimages()
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->freeallimages();
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_freeimage(MEDLAY_IMAGE imageref)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->freeimage(imageref);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}
/*
bool ctmedlay_setimagetodisplay(MEDLAY_IMAGE imageref, int pos_x, int pos_y)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->setimagetodisplay(imageref, pos_x, pos_y);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}
*/
bool ctmedlay_setimagetodisplay(MEDLAY_IMAGE imageref, CTVEC2 pos, CRECT* rect)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->setimagetodisplay(imageref, pos.cx, pos.cy, rect);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}
/*
bool ctmedlay_setimagetodisplay_adv(MEDLAY_IMAGE imageref, int pos_x, int pos_y, double angle)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->setimagetodisplay_adv(imageref, pos_x, pos_y, angle);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}
*/
bool ctmedlay_setimagetodisplay_adv(MEDLAY_IMAGE imageref, CTVEC2 pos, double angle)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->setimagetodisplay_adv(imageref, pos.cx, pos.cy, angle);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

_DLLFUNC bool ctmedlay_getimagesize(MEDLAY_IMAGE imageref, CTVEC2* size)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		int cx = 0;
		int cy = 0;
		result = ml->getimagesize(imageref, &cx, &cy);
		if (result)
		{
			*size = CTVEC2(cx, cy);
		}
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_setalphamodulation(MEDLAY_IMAGE imageref, int value)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->setalphamodulation(imageref, value);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_cleardisplay()
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->cleardisplay();
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_displayimages()
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->displayimages();
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_addfont(std::string fontfile, int fontsize, MEDLAY_FONT* fontref)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->addfont(fontfile, fontsize, fontref);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_freeallfonts()
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->freeallfonts();
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_freefont(MEDLAY_FONT fontref)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->freefont(fontref);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_texttoimage(MEDLAY_IMAGE* imageref, MEDLAY_FONT fontref, std::string text, CTCOLOR col)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->texttoimage(imageref, fontref, text, col);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_addmusic(std::string wavfile, MEDLAY_MUSIC* musicref)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->addmusic(wavfile, musicref);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_freeallmusic()
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->freeallmusic();
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_freemusic(MEDLAY_MUSIC musicref)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->freemusic(musicref);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_playmusic(MEDLAY_MUSIC musicref)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->playmusic(musicref);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_stopmusic()
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->stopmusic();
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_pausemusic()
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->pausemusic();
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_resumemusic()
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->resumemusic();
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_setmusicvolume(int volume)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->setmusicvolume(volume);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_addsound(std::string wavfile, MEDLAY_SOUND* soundref)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->addsound(wavfile, soundref);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_freeallsound()
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->freeallsound();
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_freesound(MEDLAY_SOUND soundref)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->freesound(soundref);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_playsound(MEDLAY_SOUND soundref)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->playsound(soundref);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}

bool ctmedlay_setsoundvolume(int volume, int channel)
{
	bool result = false;

	if (g_debuglog.isactive())
	{
		snprintf(g_logtext, 256, "%s() ->", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}

	if (ml)
	{
		result = ml->setsoundvolume(volume, channel);
	}

	if (g_debuglog.isactive())
	{
		if (result)	snprintf(g_logtext, 256, "%s() OK", __func__);
		else		snprintf(g_logtext, 256, "%s() NG", __func__);
		g_debuglog.writeentrytolog(LEVEL_BASIC, g_logtext);
	}
	return result;
}



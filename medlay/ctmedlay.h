#pragma once

// interface for CTMEDLAY

#include <string>

#include "ctcolor.h"
#include "ctvec2.h"


#ifdef WIN32
#ifdef	__cplusplus
#define _DLLFUNC	extern "C" __declspec(dllexport)
#else
#define _DLLFUNC	__declspec(dllexport)
#endif
#else	// WIN32
#define _DLLFUNC	
#endif

#if defined(_MSC_VER)
#ifdef LIBMEDLAY_EXPORTS
#define _DLLSTDCALL
#else
#define _DLLSTDCALL __declspec(dllimport)
#endif
#elif defined(__GNUC__)
	//  GCC
#ifdef LIBMEDLAY_EXPORTS
#define _DLLSTDCALL __attribute__ ((visibility("default")))
#else
#define _DLLSTDCALL 
#endif
#else
	//  do nothing and hope for the best?
#define _DLLSTDCALL 
#pragma warning Unknown dynamic link export/import semantics.
#endif


enum class EVENTTYPE
{
	E_QUIT = 0x100, /**< User-requested quit */
	/* Window events */
	E_WINDOWEVENT = 0x200, /**< Window state change */
	E_SYSWMEVENT,             /**< System specific event */

	/* Keyboard events */
	E_KEYDOWN = 0x300, /**< Key pressed */
	E_KEYUP,                  /**< Key released */
	E_TEXTEDITING,            /**< Keyboard text editing (composition) */
	E_TEXTINPUT,              /**< Keyboard text input */
	E_KEYMAPCHANGED,          /**< Keymap changed due to a system event such as an
	input language or keyboard layout change.
	*/

	/* Mouse events */
	E_MOUSEMOTION = 0x400, /**< Mouse moved */
	E_MOUSEBUTTONDOWN,        /**< Mouse button pressed */
	E_MOUSEBUTTONUP,          /**< Mouse button released */
	E_MOUSEWHEEL,             /**< Mouse wheel motion */

	/* Joystick events */
	E_JOYAXISMOTION = 0x600, /**< Joystick axis motion */
	E_JOYBALLMOTION,          /**< Joystick trackball motion */
	E_JOYHATMOTION,           /**< Joystick hat position change */
	E_JOYBUTTONDOWN,          /**< Joystick button pressed */
	E_JOYBUTTONUP,            /**< Joystick button released */
	E_JOYDEVICEADDED,         /**< A new joystick has been inserted into the system */
	E_JOYDEVICEREMOVED,       /**< An opened joystick has been removed */

	/* Game controller events */
	E_CONTROLLERAXISMOTION = 0x650, /**< Game controller axis motion */
	E_CONTROLLERBUTTONDOWN,          /**< Game controller button pressed */
	E_CONTROLLERBUTTONUP,            /**< Game controller button released */
	E_CONTROLLERDEVICEADDED,         /**< A new Game controller has been inserted into the system */
	E_CONTROLLERDEVICEREMOVED,       /**< An opened Game controller has been removed */
	E_CONTROLLERDEVICEREMAPPED,      /**< The controller mapping was updated */

	/* Touch events */
	E_FINGERDOWN = 0x700,
	E_FINGERUP,
	E_FINGERMOTION,

	/* Gesture events */
	E_DOLLARGESTURE = 0x800,
	E_DOLLARRECORD,
	E_MULTIGESTURE,

	/* Clipboard events */
	E_CLIPBOARDUPDATE = 0x900, /**< The clipboard changed */

	/* Drag and drop events */
	E_DROPFILE = 0x1000, /**< The system requests a file open */
	E_DROPTEXT,                 /**< text/plain drag-and-drop event */
	E_DROPBEGIN,                /**< A new set of drops is beginning (NULL filename) */
	E_DROPCOMPLETE,             /**< Current set of drops is now complete (NULL filename) */

	/* Audio hotplug events */
	E_AUDIODEVICEADDED = 0x1100, /**< A new audio device is available */
	E_AUDIODEVICEREMOVED,        /**< An audio device has been removed. */

	/* Render events */
	E_RENDER_TARGETS_RESET = 0x2000, /**< The render targets have been reset and their contents need to be updated */
	E_RENDER_DEVICE_RESET, /**< The device has been reset and all textures need to be recreated */

	/** Events ::E_USEREVENT through ::E_LASTEVENT are for your use,
	*  and should be allocated with E_RegisterEvents()
	*/
	E_USEREVENT = 0x8000,

	/**
	*  This last event is only for bounding internal arrays
	*/
	E_LASTEVENT = 0xFFFF
};

enum class EVENTKEY
{
	K_UNKNOWN = 0,

	K_RETURN = '\r',
	K_ESCAPE = '\033',
	K_BACKSPACE = '\b',
	K_TAB = '\t',
	K_SPACE = ' ',
	K_EXCLAIM = '!',
	K_QUOTEDBL = '"',
	K_HASH = '#',
	K_PERCENT = '%',
	K_DOLLAR = '$',
	K_AMPERSAND = '&',
	K_QUOTE = '\'',
	K_LEFTPAREN = '(',
	K_RIGHTPAREN = ')',
	K_ASTERISK = '*',
	K_PLUS = '+',
	K_COMMA = ',',
	K_MINUS = '-',
	K_PERIOD = '.',
	K_SLASH = '/',
	K_ARROWUP = (82 | (1 << 30)),
	K_ARROWDOWN = (81 | (1 << 30)),
	K_ARROWLEFT = (80 | (1 << 30)),
	K_ARROWRIGHT = (79 | (1 << 30)),
	K_0 = '0',
	K_1 = '1',
	K_2 = '2',
	K_3 = '3',
	K_4 = '4',
	K_5 = '5',
	K_6 = '6',
	K_7 = '7',
	K_8 = '8',
	K_9 = '9',
	K_COLON = ':',
	K_SEMICOLON = ';',
	K_LESS = '<',
	K_EQUALS = '=',
	K_GREATER = '>',
	K_QUESTION = '?',
	K_AT = '@',
	/*
	   Skip uppercase letters
	 */
	K_LEFTBRACKET = '[',
	K_BACKSLASH = '\\',
	K_RIGHTBRACKET = ']',
	K_CARET = '^',
	K_UNDERSCORE = '_',
	K_BACKQUOTE = '`',
	K_a = 'a',
	K_b = 'b',
	K_c = 'c',
	K_d = 'd',
	K_e = 'e',
	K_f = 'f',
	K_g = 'g',
	K_h = 'h',
	K_i = 'i',
	K_j = 'j',
	K_k = 'k',
	K_l = 'l',
	K_m = 'm',
	K_n = 'n',
	K_o = 'o',
	K_p = 'p',
	K_q = 'q',
	K_r = 'r',
	K_s = 's',
	K_t = 't',
	K_u = 'u',
	K_v = 'v',
	K_w = 'w',
	K_x = 'x',
	K_y = 'y',
	K_z = 'z',

};

struct CRECT
{
	int x;
	int y;
	int w;
	int h;
};

typedef void* MEDLAY_IMAGE;
typedef void* MEDLAY_FONT;
typedef void* MEDLAY_MUSIC;
typedef void* MEDLAY_SOUND;

// MANAGEMENT

_DLLFUNC bool ctmedlay_initialize(std::string windowname, int screenx, int screeny);
_DLLFUNC bool ctmedlay_uninitialize();
_DLLFUNC bool ctmedlay_getscreensize(CTVEC2* size);
_DLLFUNC bool ctmedlay_pollevent(EVENTTYPE* eventtype);
_DLLFUNC bool ctmedlay_geteventkey(EVENTKEY* eventkey);
_DLLFUNC bool ctmedlay_getmouseposition(CTVEC2* pos);
_DLLFUNC int  ctmedlay_getticks();
_DLLFUNC bool ctmedlay_sleep(int milliseconds);

// GRAPHICS

_DLLFUNC bool ctmedlay_drawline(CTVEC2 a, CTVEC2 b, CTCOLOR col);
_DLLFUNC bool ctmedlay_addimage(std::string bmpfile, MEDLAY_IMAGE* imageref, bool transparentbackground);
_DLLFUNC bool ctmedlay_addimage_v11(std::string bmpfile, MEDLAY_IMAGE* imageref, CTCOLOR* transparentcolor = nullptr);
_DLLFUNC bool ctmedlay_arraytoimage(uint32_t* pixels, MEDLAY_IMAGE* imageref, int pitch);
_DLLFUNC bool ctmedlay_freeallimages();
_DLLFUNC bool ctmedlay_freeimage(MEDLAY_IMAGE imageref);
_DLLFUNC bool ctmedlay_setimagetodisplay(MEDLAY_IMAGE imageref, CTVEC2 pos, CRECT* srcrect = nullptr);
_DLLFUNC bool ctmedlay_setimagetodisplay_adv(MEDLAY_IMAGE imageref, CTVEC2 pos, double angle);
_DLLFUNC bool ctmedlay_getimagesize(MEDLAY_IMAGE imageref, CTVEC2* size);
_DLLFUNC bool ctmedlay_setalphamodulation(MEDLAY_IMAGE imageref, int value);
_DLLFUNC bool ctmedlay_cleardisplay();
_DLLFUNC bool ctmedlay_displayimages();

// TEXT

_DLLFUNC bool ctmedlay_addfont(std::string fontfile, int fontsize, MEDLAY_FONT* fontref);
_DLLFUNC bool ctmedlay_freeallfonts();
_DLLFUNC bool ctmedlay_freefont(MEDLAY_FONT fontref);
_DLLFUNC bool ctmedlay_texttoimage(MEDLAY_IMAGE* imageref, MEDLAY_FONT fontref, std::string text, CTCOLOR col);

// AUDIO

_DLLFUNC bool ctmedlay_addmusic(std::string wavfile, MEDLAY_MUSIC* musicref);
_DLLFUNC bool ctmedlay_freeallmusic();
_DLLFUNC bool ctmedlay_freemusic(MEDLAY_MUSIC musicref);
_DLLFUNC bool ctmedlay_playmusic(MEDLAY_MUSIC musicref);
_DLLFUNC bool ctmedlay_stopmusic();
_DLLFUNC bool ctmedlay_pausemusic();
_DLLFUNC bool ctmedlay_resumemusic();
_DLLFUNC bool ctmedlay_setmusicvolume(int volume);

_DLLFUNC bool ctmedlay_addsound(std::string wavfile, MEDLAY_SOUND* soundref);
_DLLFUNC bool ctmedlay_freeallsound();
_DLLFUNC bool ctmedlay_freesound(MEDLAY_SOUND soundref);
_DLLFUNC bool ctmedlay_playsound(MEDLAY_SOUND soundref);
_DLLFUNC bool ctmedlay_setsoundvolume(int volume, int channel);


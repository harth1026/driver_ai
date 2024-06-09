#ifndef PGLOGGER_H
#define PGLOGGER_H

#include <cstdio>
#include <ctime>
#include <string>

#if LINUX
#include <string.h>
#endif

#define PGLOGGER_VERSION	"1.0.1"

enum PGLOGGERLEVEL
{
	LEVEL_NONE = 0,
	LEVEL_BASIC,		// GENERAL INFO
	LEVEL_ADVANCE,		// SPECIFIC INFO
	LEVEL_EVERYTHING,	// EVERYTHING
};

static int _pgloggerlevel = 0;

static bool pglogger_getversion(char* version, size_t version_size)
{
	bool result = false;
	if (version)
	{
#ifdef LINUX
		strncpy(version, PGLOGGER_VERSION, version_size);
#else
		strcpy_s(version, version_size, PGLOGGER_VERSION);
#endif

		result = true;
	}

	return result;
}

class pglogger
{
private:
	std::FILE* m_logfile;
	std::time_t	m_start;

public:
	pglogger() { m_logfile = nullptr;	m_start = 0; }
	~pglogger() { if (m_logfile) fclose(m_logfile); }

	bool enablelog(std::string logpath, int level)
	{
		// DEBUG

		if (m_logfile)
		{
			fclose(m_logfile);
			m_logfile = nullptr;
		}

		m_start = std::clock();
#if LINUX
		m_logfile = fopen(logpath.c_str(), "w");
#else
		fopen_s(&m_logfile, logpath.c_str(), "w");
#endif
		if (m_logfile)
		{
			_pgloggerlevel = level;
			return true;
		}

		return false;
	}

	void disablelog()
	{
		if (m_logfile)
		{
			fclose(m_logfile);
			m_logfile = nullptr;
		}
	}

	bool isactive()
	{
		return (m_logfile != nullptr);
	}

	void writeentrytolog(int level, std::string entry)
	{
		if (level <= LEVEL_NONE)
			return;

		if (_pgloggerlevel >= level)
		{
			if (m_logfile)
			{
				char temptext[128];
				float functiontime = (float)(std::clock() - m_start) / 1000.f;

				snprintf(temptext, 128, "%f %s\n", functiontime, entry.c_str());

				fprintf(m_logfile, temptext);
				fflush(m_logfile);
			}
		}
	}
};

#endif // PGLOGGER_H


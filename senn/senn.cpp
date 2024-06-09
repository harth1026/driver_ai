

#include "senn.h"
//#include "ctnet.h"

#include <string.h>
#include <format>

#include "../utilities/pglogger.h"

static pglogger* pglog = nullptr;

#define VERSION					"1.0.0"
#define RELEASE					"12/01/23"

#define DEFAULT_BASESIZE		4
#define DEFAULT_CLUSTERSIZE		100
#define DEFAULT_INPUTS			1
#define DEFAULT_OUTPUTS			1

#define CHANGEWEIGHT_PERCENT	25

#define LOGLEVEL				LEVEL_NONE




bool senn_getversion(char* version, size_t version_size, char* release, size_t release_size)
{
	bool result = false;
	if (version && release)
	{
	#ifdef LINUX
		strncpy(version, VERSION, version_size);
		strncpy(release, RELEASE, release_size);
	#else
		strcpy_s(version, version_size, VERSION);
		strcpy_s(release, release_size, RELEASE);
	#endif

		result = true;
	}

	return result;
}

bool senn_initialize(HENN* ph)
{
	bool result = false;

	if (pglog)
	{
		pglog->disablelog();
		delete pglog;
		pglog = nullptr;
	}

	if (LOGLEVEL > 0)
	{
		pglog = new pglogger();
		pglog->enablelog("sennlog.txt", LOGLEVEL);
	}
	pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} -> ", __FUNCTION__));

	if(ph)
	{
		memset(ph, 0, sizeof(HENN));

		ph->basesize = DEFAULT_BASESIZE;
		ph->clustersize = DEFAULT_CLUSTERSIZE;

		ph->networkinputs = DEFAULT_INPUTS;
		ph->networkoutputs = DEFAULT_OUTPUTS;

		ph->base = nullptr;
		ph->cluster = nullptr;
		ph->clustersize = 0;

		ph->mutation_totaloptions = 30;
		ph->mutation_input_add = 3;
		ph->mutation_input_remove = 1;
		ph->mutation_node_add = 1;
		ph->mutation_node_remove = 0;


		ph->status = SENN_STATUS::UNSTABLE;

		result = true;

	}

	if (result)
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} OK", __FUNCTION__));
	}
	else
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} NG", __FUNCTION__));
	}

	return result;
}

bool senn_uninitialize(HENN &h)
{
	bool result = true;

	pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} -> ", __FUNCTION__));

	if(h.cluster)
	{
		delete [] h.cluster;
		h.cluster = nullptr;
	}

	if(h.base)
	{
		delete [] h.base;
		h.base = nullptr;
	}

	if (result)
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} OK", __FUNCTION__));
	}
	else
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} NG", __FUNCTION__));
	}

	if (pglog)
	{
		pglog->disablelog();
		delete pglog;
		pglog = nullptr;
	}

	return true;
}

bool senn_setpropertyvalue(HENN &h, CTPROP propid, int value)
{
	bool result = false;

	pglog->writeentrytolog(LEVEL_BASIC, std::format("{} -> propid={} value={}", __FUNCTION__, (int)propid, value));

	if(h.status == SENN_STATUS::UNSTABLE)
	{
		switch(propid)
		{
		case CTPROP::PID_INPUTS:
			h.networkinputs = value;
			result = true;
			break;

		case CTPROP::PID_OUTPUTS:
			h.networkoutputs = value;
			result = true;
			break;

		case CTPROP::PID_BASESIZE:
			h.basesize = value;
			result = true;
			break;

		case CTPROP::PID_CLUSTERSIZE:
			h.clustersize = value;
			result = true;
			break;

		case CTPROP::PID_ACTIVATIONTYPE:
			break;

		case CTPROP::PID_WEIGHTCHANGEPERCENTAGE:
			h.changepercent = value;
			result = true;
			break;

		case CTPROP::PID_MUTATION_TOTALOPTIONS:
			h.mutation_totaloptions = value;
			result = true;
			break;

		case CTPROP::PID_MUTATION_INPUT_ADD_OPTIONS:
			h.mutation_input_add = value;
			result = true;
			break;

		case CTPROP::PID_MUTATION_INPUT_REMOVE_OPTIONS:
			h.mutation_input_remove = value;
			result = true;
			break;

		case CTPROP::PID_MUTATION_NODE_ADD_OPTIONS:
			h.mutation_node_add = value;
			result = true;
			break;

		case CTPROP::PID_MUTATION_NODE_REMOVE_OPTIONS:
			h.mutation_node_remove = value;
			result = true;
			break;
		}
	}

	if (result)
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} OK", __FUNCTION__));
	}
	else
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} NG", __FUNCTION__));
	}

	return result;
}

bool senn_getpropertyvalue(HENN &h, CTPROP propid, int *value)
{
	bool result = false;

	pglog->writeentrytolog(LEVEL_BASIC, std::format("{} -> propid={}", __FUNCTION__, (int)propid));

	if(value)
	{
		switch(propid)
		{
		case CTPROP::PID_INPUTS:
			*value = h.networkinputs;
			result = true;
			break;

		case CTPROP::PID_OUTPUTS:
			*value = h.networkoutputs;
			result = true;
			break;

		case CTPROP::PID_BASESIZE:
			*value = h.basesize;
			result = true;
			break;

		case CTPROP::PID_CLUSTERSIZE:
			*value = h.clustersize;
			result = true;
			break;

		case CTPROP::PID_ACTIVATIONTYPE:
			break;

		case CTPROP::PID_WEIGHTCHANGEPERCENTAGE:
			*value = h.changepercent;
			result = true;
			break;
		}
	}

	if (result)
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{} OK value={}", __FUNCTION__, *value));
	}
	else
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} NG", __FUNCTION__));
	}

	return result;
}


bool senn_setnetworkattributes(HENN &h, int inputs, int outputs)
{
	bool result = false;

	pglog->writeentrytolog(LEVEL_BASIC, std::format("{} -> inputs={} outputs={}", __FUNCTION__, inputs, outputs));

	switch(h.status)
	{
	case SENN_STATUS::READY:
		if(h.cluster)
		{
			delete [] h.cluster;
			h.cluster = nullptr;
		}
		h.status = SENN_STATUS::STABLE;

	case SENN_STATUS::STABLE:
		if(h.base)
		{
			delete [] h.base;
			h.base = nullptr;
		}
		h.status = SENN_STATUS::UNSTABLE;

	case SENN_STATUS::UNSTABLE:
		h.networkinputs = inputs;
		h.networkoutputs = outputs;
		h.status = SENN_STATUS::UNSTABLE;
		result = true;
		break;

	case SENN_STATUS::BUSY:
	case SENN_STATUS::FAIL:
		break;
	}

	if (result)
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} OK", __FUNCTION__));
	}
	else
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} NG", __FUNCTION__));
	}

	return result;
}

bool senn_configuremutationratio(HENN& h, int totaloptions, int input_add, int input_remove, int node_add, int node_remove)
{
	bool result = false;

	pglog->writeentrytolog(LEVEL_BASIC, std::format("{} -> totaloptions={} input_add={} input_remove={} node_add={} node_remove={}", __FUNCTION__, totaloptions, input_add, input_remove, node_add, node_remove));

	for (int i = 0; i < h.basesize; i++)
	{
		result = h.base[i].configuremutationratio(totaloptions, input_add, input_remove, node_add, node_remove);
		if (!result)
			break;
	}

	if (result)
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} OK", __FUNCTION__));
	}
	else
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} NG", __FUNCTION__));
	}
	return result;
}


bool senn_setbasesize(HENN &h, int size)
{
	bool result = false;

	pglog->writeentrytolog(LEVEL_BASIC, std::format("{} -> size={}", __FUNCTION__, size));

	switch(h.status)
	{
	case SENN_STATUS::READY:
		if(h.cluster)
		{
			delete [] h.cluster;
			h.cluster = nullptr;
		}
		h.status = SENN_STATUS::STABLE;

	case SENN_STATUS::STABLE:
		if(h.base)
		{
			delete [] h.base;
			h.base = nullptr;
		}
		h.status = SENN_STATUS::UNSTABLE;

	case SENN_STATUS::UNSTABLE:
		h.basesize = size;
		result = true;
		h.status = SENN_STATUS::UNSTABLE;
		break;

	case SENN_STATUS::BUSY:
	case SENN_STATUS::FAIL:
		break;
	}

	if (result)
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} OK", __FUNCTION__));
	}
	else
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} NG", __FUNCTION__));
	}

	return result;
}

bool senn_setclustersize(HENN &h, int size)
{
	bool result = false;

	pglog->writeentrytolog(LEVEL_BASIC, std::format("{} -> size={}", __FUNCTION__, size));

	switch(h.status)
	{
	case SENN_STATUS::READY:
		if(h.cluster)
		{
			delete [] h.cluster;
			h.cluster = nullptr;
		}
		h.status = SENN_STATUS::STABLE;
	case SENN_STATUS::STABLE:
		if(h.base)
		{
			delete [] h.base;
			h.base = nullptr;
		}
		h.status = SENN_STATUS::UNSTABLE;

	case SENN_STATUS::UNSTABLE:
		h.clustersize = size;
		h.status = SENN_STATUS::UNSTABLE;
		result = true;
		break;

	case SENN_STATUS::BUSY:
	case SENN_STATUS::FAIL:
		break;
	}

	if (result)
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} OK", __FUNCTION__));
	}
	else
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} NG", __FUNCTION__));
	}

	return result;
}

bool senn_initializebase(HENN &h)
{
	bool result = false;

	pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} -> ", __FUNCTION__));

	switch(h.status)
	{
	case SENN_STATUS::READY:
		if(h.cluster)
		{
			delete [] h.cluster;
			h.cluster = nullptr;
		}
		h.status = SENN_STATUS::STABLE;
	case SENN_STATUS::STABLE:
		if(h.base)
		{
			delete [] h.base;
			h.base = nullptr;
		}
		h.status = SENN_STATUS::UNSTABLE;

	case SENN_STATUS::UNSTABLE:

		h.base = new ctnet[h.basesize];
		if(h.base)
		{
			// Initialize BASE 0
			result = h.base[0].initialize(h.networkinputs, h.networkoutputs, h.changepercent);

			// Copy to other base networks
			for(int i=1; i<h.basesize; i++)
			{
				h.base[i].copy(h.base[0]);
				h.base[i].mutate(mutationtype::WEIGHT_CHANGE);
			}
		}

		h.status = SENN_STATUS::STABLE;
		break;

	case SENN_STATUS::BUSY:
	case SENN_STATUS::FAIL:
		break;
	}

	if (result)
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} OK", __FUNCTION__));
	}
	else
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} NG", __FUNCTION__));
	}
	return result;
}

struct bestenn
{
	int index;
	float fitness;
};

bool senn_selectbasenetworks(HENN &h)
{
	bool result = false;

	pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} -> ", __FUNCTION__));

	if(h.status == SENN_STATUS::READY)
	{
		bestenn* be = new bestenn[h.basesize];

		int lowestindex = -1000000;
		float lowestfitness = 1000000;
		for(int i=0; i<h.basesize; i++)
		{
			be[i].index = i;
			be[i].fitness = h.cluster[i].m_fitness;
			if (lowestfitness > be[i].fitness)
			{
				lowestfitness = be[i].fitness;
				lowestindex   = i;
			}
		}

		for(int i=h.basesize; i<h.clustersize; i++)
		{
			if(h.cluster[i].m_fitness > lowestfitness)
			{
				be[lowestindex].index = i;
				be[lowestindex].fitness = h.cluster[i].m_fitness;

				// SET NEW LOWEST INDEX
				lowestindex = 0;
				lowestfitness = be[0].fitness;
				for(int basei=1; basei<h.basesize; basei++)
				{
					if (lowestfitness > be[basei].fitness)
					{
						lowestfitness = be[basei].fitness;
						lowestindex   = basei;
					}
				}
			}
		}

		// COPY BEST NETWORKS TO BASE
		for(int i=0; i<h.basesize; i++)
		{
			h.base[i].copy(h.cluster[be[i].index]);
		}

		delete[] be;
		result = true;
	}

	if (result)
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} OK", __FUNCTION__));
}
	else
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} NG", __FUNCTION__));
	}
	return result;
}

bool senn_createclusterfrombase(HENN &h)
{
	bool result = false;

	pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} -> ", __FUNCTION__));

	if(h.status == SENN_STATUS::READY)
	{
		if(h.cluster)
		{
			delete [] h.cluster;
			h.cluster = nullptr;
		}
		h.status = SENN_STATUS::STABLE;
	}
	int baseindex = 0;

	if(h.status == SENN_STATUS::STABLE)
	{
		h.cluster = new ctnet[h.clustersize];
		if(h.cluster)
		{
			// COPY LAST WINNERS BACK INTO THE CLUSTER
			for(int i=0; i<h.basesize; i++)
			{
				h.cluster[i].copy(h.base[i]);
			}

			// CREATE NEW NETWORKS FROM THE WINNERS
			for(int i=h.basesize; i<h.clustersize; i++)
			{
				if (baseindex >= h.basesize)
					baseindex = 0;

				h.cluster[i].copy(h.base[baseindex]);
				h.cluster[i].mutate(mutationtype::RANDOM);
				baseindex++;
			}
			h.status = SENN_STATUS::READY;
			result = true;
		}
	}

	if (result)
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} OK", __FUNCTION__));
	}
	else
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} NG", __FUNCTION__));
	}
	return result;
}

bool senn_destroycluster(HENN &h)
{
	bool result = false;

	pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} -> ", __FUNCTION__));

	if(h.status == SENN_STATUS::READY)
	{
		if(h.cluster)
		{
			delete [] h.cluster;
			h.cluster = nullptr;
		}
		h.status = SENN_STATUS::STABLE;
		result = true;
	}

	if (result)
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} OK", __FUNCTION__));
	}
	else
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} NG", __FUNCTION__));
	}

	return result;
}

bool senn_destroybase(HENN &h)
{
	bool result = false;

	pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} -> ", __FUNCTION__));

	if(h.status == SENN_STATUS::STABLE)
	{
		if(h.base)
		{
			delete [] h.base;
			h.base = nullptr;
		}
		h.status = SENN_STATUS::UNSTABLE;
		result = true;
	}

	if (result)
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} OK", __FUNCTION__));
	}
	else
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} NG", __FUNCTION__));
	}
	return result;
}

bool senn_feedforward(HENN &h, int clusterindex, const std::vector<double>& input, std::vector<double>& output)
{
	bool result = false;

	if (pglog && pglog->isactive())
	{
		std::string str;
		for (int i = 0; i < h.networkinputs; i++)
		{
			str += std::format("I{}={} ", i, input[i]);
		}
		pglog->writeentrytolog(LEVEL_EVERYTHING, std::format("{} -> ", __FUNCTION__) + str);
	}
	if(h.status == SENN_STATUS::READY)
	{
		if(clusterindex < h.clustersize)
		{
			result = h.cluster[clusterindex].feedforward(input, output);
		}
	}

	if (pglog && pglog->isactive())
	{
		if (result)
		{
			std::string str;
			for (int i = 0; i < h.networkoutputs; i++)
			{
				str += std::format("O{}={} ", i, output[i]);
			}
			pglog->writeentrytolog(LEVEL_EVERYTHING, std::format("{0} OK ", __FUNCTION__) + str);
		}
		else
		{
			pglog->writeentrytolog(LEVEL_EVERYTHING, std::format("{0} NG", __FUNCTION__));
		}
	}
	return result;
}

bool senn_feedforward_v2(HENN& h, int clusterindex, const double input[], int num_inputs, double output[], int* num_outputs)
{
	bool result = false;

	if (pglog && pglog->isactive())
	{
		std::string str;
		for (int i = 0; i < h.networkinputs; i++)
		{
			str += std::format("I{}={} ", i, input[i]);
		}
		pglog->writeentrytolog(LEVEL_EVERYTHING, std::format("{} -> ", __FUNCTION__) + str);
	}

	if (h.status == SENN_STATUS::READY)
	{
		if (clusterindex < h.clustersize)
		{
			std::vector<double> myinput;
			std::vector<double> myoutput;
			for (int i = 0; i < num_inputs; i++)
			{
				myinput.push_back(input[i]);
			}

			myoutput.clear();

			result = h.cluster[clusterindex].feedforward(myinput, myoutput);

			*num_outputs = (int)myoutput.size();
			for (int i = 0; i < *num_outputs; i++)
			{
				output[i] = myoutput[i];
			}

			result = true;
		}
	}

	if (pglog && pglog->isactive())
	{
		if (result)
		{
			std::string str;
			for (int i = 0; i < h.networkoutputs; i++)
			{
				str += std::format("O{}={} ", i, output[i]);
			}
			pglog->writeentrytolog(LEVEL_EVERYTHING, std::format("{0} OK ", __FUNCTION__) + str);
		}
		else
		{
			pglog->writeentrytolog(LEVEL_EVERYTHING, std::format("{0} NG", __FUNCTION__));
		}
	}
	return result;
}


bool senn_addfitness(HENN& h, int clusterindex, float fitness)
{
	bool result = false;

	pglog->writeentrytolog(LEVEL_BASIC, std::format("{} -> clusterindex={} fitness={}", __FUNCTION__, clusterindex, fitness));

	if (h.status == SENN_STATUS::READY)
	{
		if (clusterindex < h.clustersize)
		{
			h.cluster[clusterindex].m_fitness += fitness;
			result = true;
		}
	}

	if (result)
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} OK", __FUNCTION__));
	}
	else
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} NG", __FUNCTION__));
	}
	return result;
}

bool senn_setfitness(HENN &h, int clusterindex, float fitness)
{
	bool result = false;

	pglog->writeentrytolog(LEVEL_BASIC, std::format("{} -> clusterindex={} fitness={}", __FUNCTION__, clusterindex, fitness));

	if(h.status == SENN_STATUS::READY)
	{
		if(clusterindex < h.clustersize)
		{
			h.cluster[clusterindex].m_fitness = fitness;
			result = true;
		}
	}

	if (result)
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} OK", __FUNCTION__));
	}
	else
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} NG", __FUNCTION__));
	}
	return result;
}

bool senn_getfitness(HENN &h, int clusterindex, float *fitness)
{

	bool result = false;

	pglog->writeentrytolog(LEVEL_BASIC, std::format("{} -> clusterindex={}", __FUNCTION__, clusterindex));

	if(h.status == SENN_STATUS::READY)
	{
		if((clusterindex < h.clustersize) && fitness)
		{
			*fitness = h.cluster[clusterindex].m_fitness;
			result = true;
		}
	}

	if (result)
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{} OK fitness={}", __FUNCTION__, *fitness));
	}
	else
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} NG", __FUNCTION__));
	}
	return result;
}

bool senn_savebasenetworktofile(HENN &h, int baseindex, std::string filepath)
{
	// if baseindex == -1, save all base networks to a single file

	bool result = false;

	pglog->writeentrytolog(LEVEL_BASIC, std::format("{} -> baseindex={} filepath={}", __FUNCTION__, baseindex, filepath));

	result = h.base[baseindex].savenetwork(filepath);

	if (result)
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} OK", __FUNCTION__));
	}
	else
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} NG", __FUNCTION__));
	}
	return result;

}

bool senn_loadbasenetworkfromfile(HENN &h, int baseindex, std::string filepath)
{
	bool result = false;

	pglog->writeentrytolog(LEVEL_BASIC, std::format("{} -> baseindex={} filepath={}", __FUNCTION__, baseindex, filepath));

	result = h.base[baseindex].loadnetwork(filepath);

	if (result)
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} OK", __FUNCTION__));
	}
	else
	{
		pglog->writeentrytolog(LEVEL_BASIC, std::format("{0} NG", __FUNCTION__));
	}
	return result;
}


bool senn_printdebug(HENN &h, int clusterindex)
{
	h.cluster[clusterindex].debug_printnetwork();
	return true;
}

bool senn_printdebug_base(HENN &h, int baseindex)
{
	h.base[baseindex].debug_printnetwork();

	return true;
}


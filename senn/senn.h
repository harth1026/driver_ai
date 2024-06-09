#ifndef SENN_H
#define SENN_H

#include <vector>
#include "ctnet.h"

enum class SENN_STATUS
{
	UNSTABLE=0,		// base and cluster empty
	STABLE,			// base created / cluster empty
	READY,			// base and cluster created
	BUSY,			// network is processing data
	FAIL,			// network is unrecoverable
};

enum class CTPROP
{
	PID_INPUTS = 1,
	PID_OUTPUTS,
	PID_BASESIZE,
	PID_CLUSTERSIZE,
	PID_ACTIVATIONTYPE,		
	PID_WEIGHTCHANGEPERCENTAGE,			// DEF: 10
	PID_MUTATION_TOTALOPTIONS,			// DEF: 30
	PID_MUTATION_INPUT_ADD_OPTIONS,		// DEF: 3
	PID_MUTATION_INPUT_REMOVE_OPTIONS,	// DEF: 1
	PID_MUTATION_NODE_ADD_OPTIONS,		// DEF: 1
	PID_MUTATION_NODE_REMOVE_OPTIONS,	// DEF: 0
};

struct HENN
{
	ctnet* base;
	ctnet* cluster;
	int basesize;
	int clustersize;
	int networkinputs;
	int networkoutputs;
	int changepercent;					// DEF: 10 LIMIT: 0-100 
	int mutation_totaloptions;
	int mutation_input_add;
	int mutation_input_remove;
	int mutation_node_add;
	int mutation_node_remove;
	SENN_STATUS status;

	HENN()
	{
		base = nullptr;
		cluster = nullptr;
		basesize = 0;
		clustersize = 0;
		networkinputs = 0;
		networkoutputs = 0;
		changepercent = 0;
		mutation_totaloptions = 30;
		mutation_input_add = 3;
		mutation_input_remove = 1;
		mutation_node_add = 1;
		mutation_node_remove = 0;
		status = SENN_STATUS::UNSTABLE;
	}
};

bool senn_getversion(char* version, size_t version_size, char* release, size_t release_size);
bool senn_initialize(HENN* ph);
bool senn_uninitialize(HENN &h);

bool senn_setpropertyvalue(HENN &h, CTPROP propid, int  value);
bool senn_getpropertyvalue(HENN &h, CTPROP propid, int *value);
bool senn_setnetworkattributes(HENN &h, int inputs, int outputs);
bool senn_configuremutationratio(HENN &h, int totaloptions, int input_add, int input_remove, int node_add, int node_remove);

bool senn_setbasesize(HENN &h, int size);
bool senn_setclustersize(HENN &h, int size);

bool senn_addfitness(HENN &h, int clusterindex, float fitness);
bool senn_setfitness(HENN &h, int clusterindex, float fitness);
bool senn_getfitness(HENN &h, int clusterindex, float *fitness);

bool senn_initializebase(HENN &h);
bool senn_selectbasenetworks(HENN &h);
bool senn_createclusterfrombase(HENN &h);
bool senn_destroycluster(HENN &h);
bool senn_destroybase(HENN &h);

bool senn_feedforward(HENN &h, int clusterindex, const std::vector<double>& input, std::vector<double>& output);
bool senn_feedforward_v2(HENN& h, int clusterindex, const double input[], int num_inputs, double output[], int *num_outputs);

bool senn_savebasenetworktofile(HENN &h, int baseindex, std::string filepath);
bool senn_loadbasenetworkfromfile(HENN &h, int baseindex, std::string filepath);

bool senn_printdebug(HENN &h, int clusterindex);
bool senn_printdebug_base(HENN &h, int baseindex);

#endif // SENN_H


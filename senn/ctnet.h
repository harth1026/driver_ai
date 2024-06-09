#ifndef CTNET_H
#define CTNET_H



#include <iostream>
#include <vector>

#include <iostream>
#include <fstream>

#include <math.h>
#include "node.h"

using namespace ctenn;

enum class mutationtype
{
	WEIGHT_CHANGE = 0,
	ADD_INPUT,
	REMOVE_INPUT,
	ADD_NODE,
	REMOVE_NODE,
	RANDOM,
};

class ctnet
{
private:
	int gettotalconnections();
	std::ofstream m_ofile;

public:
	int m_nextid;
	int m_inputs;
	int m_outputs;
	float m_fitness;
	int m_percentage;

	int m_mutation_totaloptions;
	int m_mutation_input_add;
	int m_mutation_input_remove;
	int m_mutation_node_add;
	int m_mutation_node_remove;
	std::vector<node> m_nodelist;

	int m_totalmutations;

	ctnet() { m_nextid = 0; m_inputs = 0; m_outputs = 0; m_fitness = 0; m_nodelist.clear(); m_percentage = 0; 
		m_mutation_totaloptions = 30;
		m_mutation_input_add = 3;
		m_mutation_input_remove = 1;
		m_mutation_node_add = 1;
		m_mutation_node_remove = 0;
		m_totalmutations = 0;
	}
    ctnet(const ctnet *e)
    {
		if (e)
		{
			m_nextid = e->m_nextid;
			m_inputs = e->m_inputs;
			m_outputs = e->m_outputs;
			m_fitness = e->m_fitness;
			m_percentage = e->m_percentage;
			m_mutation_totaloptions = e->m_mutation_totaloptions;
			m_mutation_input_add = e->m_mutation_input_add;
			m_mutation_input_remove = e->m_mutation_input_remove;
			m_mutation_node_add = e->m_mutation_node_add;
			m_mutation_node_remove = e->m_mutation_node_remove;
			m_totalmutations = e->m_totalmutations;

			for (size_t i = 0; i < e->m_nodelist.size(); i++)
			{
				m_nodelist.push_back(e->m_nodelist[i]);
			}
		}
		else
		{
			m_nextid = 0; 
			m_inputs = 0; 
			m_outputs = 0; 
			m_fitness = 0; 
			m_percentage = 0;
			m_mutation_totaloptions = 30;
			m_mutation_input_add = 3;
			m_mutation_input_remove = 1;
			m_mutation_node_add = 1;
			m_mutation_node_remove = 0;
			m_totalmutations = 0;
			m_nodelist.clear();
		}
    }

    ~ctnet() { m_nodelist.clear(); }

	bool initialize(int inputs, int outputs, int percent);
	bool initialize_flat(int inputs, int outputs, int percent);

	bool feedforward(const std::vector<double>& input, std::vector<double>& output);
	bool mutate(mutationtype type);
	bool fullreset();
	bool savenetwork(std::string filename);
	bool loadnetwork(std::string filename);

	void debug_printnetwork();

	bool getnewvalue(int index, double* value);
	bool mutateconnectionweight();
	bool mutateaddinput();
	bool mutateremoveinput();
	bool mutateaddnode();
	bool mutateremovenode();
	bool setnodelevel_recursive(int id, int level);
	bool recalculatelevels();

	bool setchangepercentage(int percentage);
	bool getchangepercentage(int* percentage);

	bool configuremutationratio(int totaloptions, int input_add, int input_remove, int node_add, int node_remove);


	bool checkinfiniteloop(int nodeid, int targetnodeid);

	bool copy(ctnet& srcenn);

	std::vector<node> getnodelist();

	// direct
	bool direct_initialize();
	bool direct_uninitialize();

};


#endif



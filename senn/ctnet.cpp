#include "ctnet.h"

#include <filesystem>
#include <assert.h>


#define LEVELOUTOFRANGE	1000
#define TOTAL_TRIES		25

double getrandomweight()
{
	// return random value between -1 and 1;
	return (rand() / double(RAND_MAX)) * 2.0f - 1.0f;
}

int ctnet::gettotalconnections()
{
	int totalconnections = 0;
	for (auto n : m_nodelist)
	{
		totalconnections += (int)n.getinputlist().size();
	}

	return totalconnections;
}

bool ctnet::initialize(int inputs, int outputs, int percent)
{
	bool result = false;

	srand((unsigned int)time(NULL));
	m_inputs = inputs;
	m_outputs = outputs;
	m_percentage = percent;

	// add inputs nodes to start of node list
	for (int i = 0; i < inputs; i++)
	{
		m_nodelist.push_back(node(m_nextid, nodetype::INPUT));
		m_nextid++;
	}

	// add output nodes next into node list
	for (int i = 0; i < outputs; i++)
	{
		m_nodelist.push_back(node(m_nextid, nodetype::OUTPUT));
		for (int j = 0; j < inputs; j++)
		{
			// add input connections from output to each input
			m_nodelist[m_nextid].addinput(j, getrandomweight());
		}

		m_nextid++;
	}

	recalculatelevels();


	result = true;
	return result;
}

bool ctnet::initialize_flat(int inputs, int outputs, int percent)
{
	bool result = false;
	srand((unsigned int)time(NULL));
	m_inputs = inputs;
	m_outputs = outputs;
	m_percentage = percent;

	// add inputs nodes to start of node list
	for (int i = 0; i < inputs; i++)
	{
		m_nodelist.push_back(node(m_nextid, nodetype::INPUT));
		m_nextid++;
	}

	// add output nodes next into node list
	for (int i = 0; i < outputs; i++)
	{
		m_nodelist.push_back(node(m_nextid, nodetype::OUTPUT));
		for (int j = 0; j < inputs; j++)
		{
			// add input connections from output to each input
			m_nodelist[m_nextid].addinput(j, 0);
		}

		m_nextid++;
	}

	recalculatelevels();
	result = true;
	return result;
}

bool ctnet::feedforward(const std::vector<double>& input, std::vector<double>& output)
{
	bool result = false;

	// test that the input size is legit
	if (m_inputs == (int)input.size())
	{
		// reset each node to unstable state
		for (size_t i = 0; i < m_nodelist.size(); i++)
		{
			m_nodelist[i].setstable(false);
		}

		// set the input values and stablize input nodes
		for (int i = 0; i < m_inputs; i++)
		{
			m_nodelist[i].setvalue(input[i]);
			m_nodelist[i].setstable(true);
		}

		output.clear();

		// calculate output nodes by recursively calculating dependent nodes
		for (int i = 0; i < m_outputs; i++)
		{
			// output nodes are just after the input nodes
			// therefore we add the number of inputs to get the index
			int outputindex = i + m_inputs;
			double sum = 0;
			getnewvalue(outputindex, &sum);
			output.push_back(tanh(sum));
		}
		if (m_outputs == (int)output.size())
			result = true;
	}

	return result;
}

bool ctnet::mutate(mutationtype type)
{
	mutationtype mtype = type;

	if (type == mutationtype::RANDOM)
	{
		int itype = rand() % m_mutation_totaloptions;

		if (itype < m_mutation_input_add)
		{
			mtype = mutationtype::ADD_INPUT;
		}
		else if (itype < (m_mutation_input_add + m_mutation_input_remove))
		{
			mtype = mutationtype::REMOVE_INPUT;
		}
		else if (itype < (m_mutation_input_add + m_mutation_input_remove + m_mutation_node_add))
		{
			mtype = mutationtype::ADD_NODE;
		}
		else
		{
			mtype = mutationtype::WEIGHT_CHANGE;
		}
	}

	bool result = false;
	switch (mtype)
	{
	case mutationtype::WEIGHT_CHANGE:
		result = mutateconnectionweight();
		recalculatelevels();
		break;
	case mutationtype::ADD_INPUT:
		result = mutateaddinput();
		recalculatelevels();
		break;
	case mutationtype::ADD_NODE:
		result = mutateaddnode();
		recalculatelevels();
		break;
	case mutationtype::REMOVE_INPUT:
		result = mutateremoveinput();
		recalculatelevels();
		break;
	case mutationtype::REMOVE_NODE:
		// TODO: figure this out... :P
		//		result = mutateremovenode();
		break;
	default: break;
	}


	return result;
}

bool ctnet::fullreset()
{
	return false;
}

bool ctnet::savenetwork(std::string filename)
{
	std::ofstream myfile;
	myfile.open(filename);

	myfile << "2\n";		// FILE FORMAT VERSION

	myfile << m_totalmutations << " ";
	myfile << m_inputs << " ";
	myfile << m_outputs << " ";
	myfile << m_nodelist.size() << "\n";

	for(auto snode : m_nodelist)
	{
		myfile << snode.getid() << " ";
		switch(snode.gettype())
		{
		case nodetype::INPUT:	myfile << "I ";		break;
		case nodetype::OUTPUT:	myfile << "O ";		break;
		case nodetype::HIDDEN:	myfile << "H ";		break;
		default:				myfile << "U ";		break;
		}
		myfile << snode.getlevel() << " ";
		myfile << snode.getinputlist().size() << " ";

		for(auto input : snode.getinputlist())
		{
			myfile << input.inputnodeid << " ";
			myfile << input.weight << " ";
		}
		myfile << "\n";
	}

	myfile.close();
	return true;
}

#include <filesystem>

bool ctnet::loadnetwork(std::string filename)
{
	bool result = false;

	bool exist = std::filesystem::exists(filename);

	if(exist)
	{
		try
		{
			m_nodelist.clear();

			std::ifstream myfile;
			myfile.open(filename);

			int nextid = 0;

			int iVal = 0;
			char cVal;
			double dVal;
			std::string sVal;


			myfile >> iVal;		// FILE FORMAT VERSION

			switch (iVal)
			{
			case 1:
				myfile >> m_inputs;
				myfile >> m_outputs;
				myfile >> nextid;

				for (int i = 0; i < nextid; i++)
				{
					node n;

					myfile >> iVal;
					n.setid(iVal);

					myfile >> cVal;
					switch (cVal)
					{
					case 'I':	n.settype(nodetype::INPUT);		break;
					case 'O':	n.settype(nodetype::OUTPUT);	break;
					case 'H':	n.settype(nodetype::HIDDEN);	break;
					default:	n.settype(nodetype::UNKNOWN);	break;
					}
					myfile >> iVal;
					n.setlevel(iVal);

					int inputs = 0;
					myfile >> inputs;	// snode.m_inputlist.size() << " ";

					for (int j = 0; j < inputs; j++)
					{
						myfile >> iVal;
						myfile >> sVal;

						dVal = std::stod(sVal);

						n.addinput(iVal, dVal);
					}

					m_nodelist.push_back(n);
				}

				m_nextid = nextid;

				break;

			case 2:
				myfile >> m_totalmutations;		// added for version 2
				myfile >> m_inputs;
				myfile >> m_outputs;
				myfile >> nextid;

				for (int i = 0; i < nextid; i++)
				{
					node n;

					myfile >> iVal;
					n.setid(iVal);

					myfile >> cVal;
					switch (cVal)
					{
					case 'I':	n.settype(nodetype::INPUT);		break;
					case 'O':	n.settype(nodetype::OUTPUT);	break;
					case 'H':	n.settype(nodetype::HIDDEN);	break;
					default:	n.settype(nodetype::UNKNOWN);	break;
					}
					myfile >> iVal;
					n.setlevel(iVal);

					int inputs = 0;
					myfile >> inputs;	// snode.m_inputlist.size() << " ";

					for (int j = 0; j < inputs; j++)
					{
						myfile >> iVal;
						myfile >> sVal;

						dVal = std::stod(sVal);

						n.addinput(iVal, dVal);
					}

					m_nodelist.push_back(n);
				}

				m_nextid = nextid;
				break;

			default:
				// INVALID FILE FORMAT
				break;
			}



			myfile.close();
			result = true;
		}
		catch (std::ios_base::failure& e)
		{
			std::cout << "Fail to read file " << e.code() << std::endl;
		}
	}

	return result;
}

void ctnet::debug_printnetwork()
{
	std::cout << "Total Mutations: " << m_totalmutations << std::endl;
	for(size_t i=0; i<m_nodelist.size(); i++)
	{
		std::cout << i;
		switch (m_nodelist[i].gettype())
		{
		case nodetype::INPUT:	std::cout << " I ";	break;
		case nodetype::OUTPUT:	std::cout << " O ";	break;
		case nodetype::HIDDEN:	std::cout << " H ";	break;
		default: break;
		}

		std::cout << m_nodelist[i].getlevel();

		std::vector<ninput> myinputlist = m_nodelist[i].getinputlist();
		for (auto myinput : myinputlist)
		{
			std::cout << " (" << myinput.inputnodeid << ":" << myinput.weight << ")";
		}
		std::cout << std::endl;
	}
}

bool ctnet::getnewvalue(int index, double *value)
{
	bool result = false;
	if(value)
	{
		// if all inputs are already calculated
		if (m_nodelist[index].isstable())
		{
			*value = m_nodelist[index].getvalue();
			return true;
		}

		*value = 0;

		double dval = 0;
		for (auto& input : m_nodelist[index].getinputlist())
		{
			getnewvalue(input.inputnodeid, &dval);
			*value += (dval * input.weight);
		}

		m_nodelist[index].setvalue(tanh(*value));
		m_nodelist[index].setstable(true);
		result = true;
	}
	return result;
}

bool ctnet::mutateconnectionweight()
{
	bool result = false;

	int connectionstochange = (gettotalconnections() * m_percentage) / 100;
	if (connectionstochange < 1)
		connectionstochange = 1;

	for (int i = 0; i < connectionstochange; i++)
	{
		// get random non-input node 
		int dstnodeid = rand() % (m_nodelist.size() - m_inputs) + m_inputs;

		// get random input connection
		int numberofinputs = (int)m_nodelist[dstnodeid].getnumberofinputs();

		if (numberofinputs > 0)
		{
			int inputindex = rand() % numberofinputs;

			// randomize random input weight
			double weight = getrandomweight();
			m_nodelist[dstnodeid].changeinputweight(inputindex, weight);
			result = true;
		}
	}

	if (result)
		m_totalmutations++;

	return result;
}

bool ctnet::mutateaddinput()
{
	bool result = false;
	bool nodefound = false;
	int tries = 0;
	int dstnodeid = -1;
	int srcnodeid = -1;
	do
	{
		tries++;

		nodefound = true;

		// get random non-input node 
		dstnodeid = rand() % (m_nodelist.size() - m_inputs) + m_inputs;

		// get random non-output node
		// favor OUTOFRANGE input nodes
		for (int i = 0; i < m_inputs; i++)
		{
			if (m_nodelist[i].getlevel() == LEVELOUTOFRANGE)
			{
				srcnodeid = i;
				break;
			}
		}

		if (srcnodeid == -1)
		{
	 		srcnodeid = rand() % (m_nodelist.size() - m_outputs);
			if (srcnodeid >= m_inputs)
				srcnodeid += m_outputs;
		}

		// check bad conditions
		if (srcnodeid == dstnodeid)		// same node
			nodefound = false;
		if (m_nodelist[srcnodeid].getlevel() <= m_nodelist[dstnodeid].getlevel())  // wrong direction
			nodefound = false;
		if (m_nodelist[dstnodeid].getlevel() == LEVELOUTOFRANGE)  // inactive destination node
			nodefound = false;
		for (auto connection : m_nodelist[dstnodeid].getinputlist())		// connection exist
		{
			if (connection.inputnodeid == srcnodeid)
			{
				nodefound = false;
				break;
			}
		}

		// () Causing an issue where no new inputs are acceptable

		// check for infinite loop
		if (checkinfiniteloop(srcnodeid, dstnodeid))
			nodefound = false;

		if (tries >= TOTAL_TRIES)
			return false;
	} while (!nodefound);

	// if code reaches this point, then inputid is not connected to outputid
	m_nodelist[dstnodeid].addinput(srcnodeid, getrandomweight());
	result = true;

	if (result)
		m_totalmutations++;

	return result;
}

bool ctnet::configuremutationratio(int totaloptions, int input_add, int input_remove, int node_add, int node_remove)
{
	bool result = false;

	if (totaloptions > (input_add + input_remove + node_add + node_remove))
	{
		// totaloptions must be greater than sum of all other options...
		m_mutation_totaloptions = totaloptions;
		m_mutation_input_add = input_add;
		m_mutation_input_remove = input_remove;
		m_mutation_node_add = node_add;
		m_mutation_node_remove = node_remove;

		result = true;
	}
	return false;
}

bool ctnet::checkinfiniteloop(int srcnodeid, int dstnodeid)
{
	// returns true if infinite loop between src and dst.

	for (auto connection : m_nodelist[srcnodeid].getinputlist())		// connection exist
	{
		// dst should not be an input of src
		if (connection.inputnodeid == dstnodeid)
			return true;

		if (checkinfiniteloop(connection.inputnodeid, dstnodeid))
			return true;
	}

	return false;
}

bool ctnet::mutateremoveinput()
{
	bool result = false;

	// get random non-input node 
	int outputid = rand() % (m_nodelist.size() - m_inputs) + m_inputs;

	// check if inputs are greater than 1
	size_t numinputs = m_nodelist[outputid].getnumberofinputs();
	if (numinputs > 1)
	{
		// select random connection
		int inputid = rand() % numinputs;

		int srcnodeid = m_nodelist[outputid].getidfrominput(inputid);
		if (srcnodeid < m_inputs)
		{
			// srcnodeid == input node
			// don't remove connect... set connection weight to zero
			m_nodelist[outputid].changeinputweight(inputid, 0.0f);
		}
		else
		{
			m_nodelist[outputid].removeinput(inputid);
		}

		m_totalmutations++;

	}
	result = true;

	return result;
}

bool ctnet::mutateaddnode()
{
	bool result = false;

	// get random non-input node
	int dstnodeid = rand() % (m_nodelist.size() - m_inputs) + m_inputs;

	// get random input connection
	int inputindex = rand() % m_nodelist[dstnodeid].getnumberofinputs();

	// get input node id of connection
	int srcnodeid = m_nodelist[dstnodeid].getidfrominput(inputindex);

	// delete connection
	m_nodelist[dstnodeid].removeinput(inputindex);

	// create new node
	m_nodelist.push_back(node(m_nextid, nodetype::HIDDEN));
	int newnodeid = m_nextid;
	m_nextid++;


	// TODO: probably add the connections without 2 recalcs

	// replace connections
	m_nodelist[dstnodeid].addinput(newnodeid, getrandomweight());

	// recalculate levels
	recalculatelevels();

	m_nodelist[newnodeid].addinput(srcnodeid, getrandomweight());

	// recalculate levels
	recalculatelevels();


#if 1 //ADDSECONDINPUT
	// get second random non-output node that isn't dstnodeid, m_nextid, or inputnodeid.
	int srcnodeid2 = 0;
	bool nodefound = false;

	int tries = 0;
	do
	{
		tries++;
		srcnodeid2 = rand() % (m_nodelist.size() - m_outputs);
		if (srcnodeid2 >= m_inputs)
			srcnodeid2 += m_outputs;
		nodefound = true;

		if (srcnodeid2 == dstnodeid)
			nodefound = false;
		if (srcnodeid2 == srcnodeid)
			nodefound = false;
		if (srcnodeid2 == newnodeid)
			nodefound = false;
		if (m_nodelist[srcnodeid2].getlevel() < m_nodelist[newnodeid].getlevel())
			nodefound = false;
		if (m_nodelist[srcnodeid2].getlevel() == LEVELOUTOFRANGE)  // inactive node
			nodefound = false;

		// check for infinite loop
		if (checkinfiniteloop(srcnodeid2, dstnodeid))
			nodefound = false;

		if (tries >= 25)
		{
			return false;
		}
	} 
	while (!nodefound);

	// create connection between new node and srcnodeid2
	m_nodelist[newnodeid].addinput(srcnodeid2, getrandomweight());

	// recalculate levels
	recalculatelevels();
#endif // ADDSECONDINPUT

	result = true;

	if (result)
		m_totalmutations++;

	return result;
}

bool ctnet::mutateremovenode()
{
	bool result = false;
	// NOT SUPPORTED YET

	return result;
}

bool ctnet::setnodelevel_recursive(int id, int level)
{
	bool result = true;

	if (level > m_nodelist.size())
	{
		// level can't be greater than number of nodes
//		assert(0);

		// TODO: Fix this properly.
		m_nodelist[id].setlevel(LEVELOUTOFRANGE);
		return true;
	}

	if (level > m_nodelist[id].getlevel())
	{
		m_nodelist[id].setlevel(level);
		std::vector<ninput> inputlist = m_nodelist[id].getinputlist();

		for (auto input : inputlist)
		{
			result = setnodelevel_recursive(input.inputnodeid, level + 1);
		}

	}

	return result;
}

// This function traverses each node and reevaluates the level value
bool ctnet::recalculatelevels()
{
	bool result = false;

	// set level of each node to -1 for calculation purposes
	for (size_t i = 0; i < m_nodelist.size(); i++)
		m_nodelist[i].setlevel(-1);

	// for each output, recursively go through each input node
	for (int i = m_inputs; i < m_inputs + m_outputs; i++)
		result = setnodelevel_recursive(i, 0);

	for (size_t i = 0; i < m_nodelist.size(); i++)
	{
		if (m_nodelist[i].getlevel() == -1)
			m_nodelist[i].setlevel(LEVELOUTOFRANGE);		// 1000 because it should be out of reach
	}
	return result;
}

bool ctnet::setchangepercentage(int percentage)
{
	m_percentage = percentage;
	return true;

}

bool ctnet::getchangepercentage(int* percentage)
{
	bool result = false;
	if (percentage)
	{
		*percentage = m_percentage;
	}
	return result;
}




bool ctnet::copy(ctnet &srcenn)
{
	bool result = false;

	m_nextid = srcenn.m_nextid;
	m_inputs = srcenn.m_inputs;
	m_outputs = srcenn.m_outputs;
	m_fitness = srcenn.m_fitness;
	m_percentage = srcenn.m_percentage;
	m_mutation_totaloptions = srcenn.m_mutation_totaloptions;
	m_mutation_input_add = srcenn.m_mutation_input_add;
	m_mutation_input_remove = srcenn.m_mutation_input_remove;
	m_mutation_node_add = srcenn.m_mutation_node_add;
	m_mutation_node_remove = srcenn.m_mutation_node_remove;
	m_totalmutations = srcenn.m_totalmutations;

	m_nodelist.clear();

	for(auto snode : srcenn.m_nodelist)
	{
		node dnode = node(snode);
		m_nodelist.push_back(dnode);
	}
	result = true;

	return result;
}

std::vector<node> ctnet::getnodelist()
{
	return m_nodelist;
}

bool ctnet::direct_initialize()
{
	return false;
}

bool ctnet::direct_uninitialize()
{
	return false;
}



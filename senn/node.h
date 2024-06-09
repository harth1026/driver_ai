#ifndef NODE_H
#define NODE_H

#include <vector>
#include <math.h>

namespace ctenn
{
	enum class nodetype
	{
		INPUT = 0,
		OUTPUT,
		HIDDEN,
		UNKNOWN,
	};

	enum activationtype
	{
		BINARY = 0,
		LINEAR,
		TANH,
		RELU,
		UNKNOWN,
	};

	struct ninput
	{
		int inputnodeid;
		double weight;
		ninput() { inputnodeid = 0; weight = 0; }
	};

	class node
	{
	private:
		int					m_id;
		bool				m_stable;
		nodetype			m_type;
		activationtype		m_activation;
		int					m_level;			// distance to closest output
		double				m_value;
		std::vector<ninput> m_inputlist;

	public:
		node()
		{
			m_id = 0;
			m_stable = false;
			m_type = nodetype::UNKNOWN;
			m_activation = activationtype::UNKNOWN;
			m_value = 0;
			m_inputlist.clear();
			m_level = 10000;
		}
		node(int id, nodetype type)
		{
			m_id = id;
			m_stable = false;
			m_type = type;
			m_activation = activationtype::UNKNOWN;
			m_value = 0;
			m_inputlist.clear();
			m_level = 10000;
		}

		node(const node& n)
		{
			m_id = n.m_id;
			m_stable = n.m_stable;
			m_type = n.m_type;
			m_activation = n.m_activation;
			m_value = n.m_value;
			m_level = n.m_level;

			ninput temp;
			m_inputlist.clear();
			for (size_t i = 0; i < n.m_inputlist.size(); i++)
			{
				temp.inputnodeid = n.m_inputlist[i].inputnodeid;
				temp.weight = n.m_inputlist[i].weight;
				m_inputlist.push_back(temp);
			}
		}

		~node() { m_inputlist.clear(); }

		void setid(int id) { m_id = id; }
		void setstable(bool stable) { m_stable = stable; }
		void settype(nodetype type) { m_type = type; }
		void setactivation(activationtype type) { m_activation = type; }
		void setvalue(double value) { m_value = value; }
		void setlevel(int level) { m_level = level; }
		int getid() { return m_id; }


		int	getnodeindex(int id)
		{
			for (size_t i = 0; i < m_inputlist.size(); i++)
			{
				if (id == m_inputlist[i].inputnodeid)
				{
					return (int)i;
				}
			}
			return -1;
		}

		int getidfrominput(int inputid)
		{
			int result = -1;
			if (inputid < (int)m_inputlist.size())
				result = m_inputlist[inputid].inputnodeid;
			return result;
		}

		double getweightfrominput(int inputid)
		{
			double result = 0.0f;
			if (inputid < m_inputlist.size())
				result = m_inputlist[inputid].weight;
			return result;
		}

		double getvalue()
		{
			double result = 0;
			if (m_stable)
			{
				result = m_value;
			}

			return result;
		}
		bool isstable() { return m_stable; }
		nodetype gettype() { return m_type; }
		activationtype getactivation() { return m_activation; }
		size_t getnumberofinputs() { return m_inputlist.size(); }
		int getlevel() { return m_level; }
		std::vector<ninput> getinputlist() { return m_inputlist; }


		void addinput(int inputnode, double weight)
		{
			ninput n;
			n.inputnodeid = inputnode;
			n.weight = weight;
			m_inputlist.push_back(n);
		}

		void removeinput(int index)
		{
			m_inputlist.erase(m_inputlist.begin() + index);
		}

		void changeinputweight(int index, double weight)
		{
			m_inputlist[index].weight = weight;
		}

		void calculate(std::vector<double> inputs)
		{
			double value = 0;

			if (inputs.size() == m_inputlist.size())
			{
				double dval = 0;
				for (int i = 0; i < m_inputlist.size(); i++)
				{
					dval += inputs[i] * m_inputlist[i].weight;
				}

				switch (m_activation)
				{
				case activationtype::BINARY:
					if (dval > 0.5f)	value = 1;
					else			value = 0;
					m_stable = true;
					break;
				case activationtype::LINEAR:
					value = dval;
					m_stable = true;
					break;
				case activationtype::TANH:
					value = tanh(dval);
					m_stable = true;
					break;
				case activationtype::RELU:
					if (dval <= 0)
						value = 0;
					else
						value = dval;
					m_stable = true;
					break;
				case activationtype::UNKNOWN:
					// Don't calculate... return 0
					value = 0;
					m_stable = true;
					break;
				}
			}

			m_value = value;
		}

		node operator=(const node& b)
		{
			node temp;
			temp.m_id = b.m_id;
			temp.m_stable = b.m_stable;
			temp.m_type = b.m_type;
			temp.m_value = b.m_value;
			temp.m_level = b.m_level;
			for (auto input : b.m_inputlist)
			{
				temp.m_inputlist.push_back(input);
			}
			return temp;
		}

		void debug_printnode()
		{
			printf("m_id = %d\n", m_id);

			if (m_stable)
				printf("m_stable = true\n");
			else
				printf("m_stable = false\n");

			switch (m_type)
			{
			case nodetype::INPUT:
				printf("m_type = INPUT\n");
				break;
			case nodetype::OUTPUT:
				printf("m_type = OUTPUT\n");
				break;
			case nodetype::HIDDEN:
				printf("m_type = HIDDEN\n");
				break;
			case nodetype::UNKNOWN:
				printf("m_type = UNKNOWN\n");
				break;
			}
			switch (m_activation)
			{
			case activationtype::BINARY:
				printf("m_activation = BINARY\n");
				break;
			case activationtype::LINEAR:
				printf("m_activation = LINEAR\n");
				break;
			case activationtype::TANH:
				printf("m_activation = TANH\n");
				break;
			case activationtype::RELU:
				printf("m_activation = RELU\n");
				break;
			case activationtype::UNKNOWN:
				printf("m_activation = UNKNOWN\n");
				break;
			}

			printf("m_level = %d\n", m_level);
			printf("m_value = %f\n", m_value);

			printf("m_inputlist.Count = %d\n", (int)m_inputlist.size());
			for (int i = 0; i < m_inputlist.size(); i++)
			{
				printf("m_inputlist[%d].inputnodeid = %d", i, m_inputlist[i].inputnodeid);
				printf("m_inputlist[%d].weight = %f", i, m_inputlist[i].weight);
			}
		}
	};
};

#endif 


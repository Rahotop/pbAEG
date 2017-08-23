#include "graph.hpp"

template<typename T>
Graph<T>::Graph() : m_nbNodes(0), m_costs(nullptr), m_costAll(0), m_mat(), m_mat0()
{

}

template<typename T>
Graph<T>::Graph(std::string path) : m_nbNodes(0), m_costs(nullptr), m_costAll(0), m_mat(), m_mat0()
{
	std::ifstream in(path.c_str());

	std::string buffer;
	char cbuffer[1024];

	in >> buffer;

	while(!in.eof())
	{
		if(buffer == "t")
		{
			int n1, n2;
			T c;
			in >> n1 >> n2 >> c;

			m_mat[n1*m_nbNodes+n2] = c;
			m_mat[n2*m_nbNodes+n1] = c;

			m_mat0[n1*m_nbNodes+n2] = c;
			m_mat0[n2*m_nbNodes+n1] = c;
		}
		else if(buffer == "c")
		{
			in.getline(cbuffer, 1024);
		}
		else if(buffer == "node")
		{
			in >> m_nbNodes;
			m_mat = Matrix<T>(m_nbNodes);
			m_mat0 = Matrix<T>(m_nbNodes);

			m_costs = new T[m_nbNodes];
			for(int i(0); i < m_nbNodes; i++)
			{
				in >> m_costs[i];
				m_costAll += m_costs[i];
			}
		}

		in >> buffer;
	}

	m_mat.floyd_warshall();
}

template<typename T>
Graph<T>::~Graph()
{
	delete[] m_costs;
}

template<typename T>
void add_sol(Solution<T> *s, std::vector<Solution<T>*>& all)
{
	if(s->is_sol() && *s <= all && !(*s == all))
	{
		for(int i(all.size()-1); i >= 0; i--)
		{
			if(*s << *(all[i]))
			{
				delete all[i];
				all.erase(all.begin()+i);
			}
		}

		all.push_back(s);
	}
	else
	{
		delete s;
	}
}

template<typename T>
void Graph<T>::local_search(int nbIt, T *w, int seed)
{
	std::vector<Solution<T>*> all;
	std::vector<Solution<T>*> evolutions;

	int restart(1);
	Solution<T> *current(new Solution<T>(*this));
	T score(current->valuation(*this, w));
	evolutions.push_back(new Solution<T>(*this, current->getSolution()));

	std::vector<int> shuffle;
	for(int i(0); i < m_nbNodes; i++)
	{
		shuffle.push_back(i);
	}

	for(int iteration(0); restart <= nbIt; iteration++)
	{
		std::random_shuffle(shuffle.begin(), shuffle.end());

		int i(0);
		for(i = 0; i < m_nbNodes; i++)
		{
			Solution<T> *sol(new Solution<T>(*this, *current, shuffle[i]));
			T tmp(sol->valuation(*this, w));

			if(tmp > score)
			{
				score = tmp;
				add_sol(current, all);
				current = sol;
				break;
			}
			else
			{
				delete sol;
			}
		}

		if(i == m_nbNodes)
		{
			std::cout << std::endl;
			iteration = 0;
			srand(seed+(restart++));
			evolutions.push_back(new Solution<T>(*this, current->getSolution()));
			add_sol(current, all);

			current = new Solution<T>(*this);
			score = current->valuation(*this, w);
			evolutions.push_back(new Solution<T>(*this, current->getSolution()));
		}

		std::cout << "Restart " << restart << ": " << iteration << "    \r";
	}
	std::cout << std::endl;

	evolutions.push_back(new Solution<T>(*this, current->getSolution()));
	add_sol(current, all);
	
	for(unsigned int i(0); i < evolutions.size(); i++)
		evolutions[i]->valuation(*this, w);


	save(all, "out");
	saveHTML(all, "out.html", sqrt(m_nbNodes), sqrt(m_nbNodes), 20);
	saveLaTex(all, "out.tex", sqrt(m_nbNodes), sqrt(m_nbNodes));
	saveHTML(evolutions, "evol.html", sqrt(m_nbNodes), sqrt(m_nbNodes), 2);

	for(unsigned int i(0); i < all.size(); i++)
	{
		delete all[i];
	}

	for(unsigned int i(0); i < evolutions.size(); i++)
	{
		delete evolutions[i];
	}
}

template<typename T>
void Graph<T>::local_search(int nbIt, T *w, int seed, T tau0, T alpha)
{
	T tau(tau0);
	std::default_random_engine gen (seed);
	std::uniform_real_distribution<T> db (0.,1.);
	
	std::vector<Solution<T>*> all;
	std::vector<Solution<T>*> evolutions;

	int restart(1);
	Solution<T> *current(new Solution<T>(*this));
	T score(current->valuation(*this, w));
	evolutions.push_back(new Solution<T>(*this, current->getSolution()));

	std::vector<int> shuffle;
	for(int i(0); i < m_nbNodes; i++)
	{
		shuffle.push_back(i);
	}

	for(int iteration(0); restart <= nbIt; iteration++)
	{
		std::random_shuffle(shuffle.begin(), shuffle.end());

		int i(0);
		for(i = 0; i < m_nbNodes; i++)
		{
			Solution<T> *sol(new Solution<T>(*this, *current, shuffle[i]));
			T tmp(sol->valuation(*this, w));

			if((tmp > score || exp((tmp-score)/tau) > db(gen)) && tmp != score)
			{
				score = tmp;
				add_sol(current, all);
				current = sol;
				break;
			}
			else
			{
				delete sol;
			}
		}

		tau *= alpha;

		if(i == m_nbNodes)
		{
			std::cout << std::endl;
			iteration = 0;
			tau = tau0;
			//restart++;/*
			srand(seed+(restart++));
			evolutions.push_back(new Solution<T>(*this, current->getSolution()));
			add_sol(current, all);

			current = new Solution<T>(*this);
			score = current->valuation(*this, w);
			evolutions.push_back(new Solution<T>(*this, current->getSolution()));//*/
		}
		std::cout << "Restart " << restart << ": " << iteration << " score = " << score << " tau = " << tau << "    \r";
	}
	std::cout << std::endl;

	evolutions.push_back(new Solution<T>(*this, current->getSolution()));
	add_sol(current, all);
	
	for(unsigned int i(0); i < evolutions.size(); i++)
		evolutions[i]->valuation(*this, w);


	save(all, "out");
	saveHTML(all, "out.html", sqrt(m_nbNodes), sqrt(m_nbNodes), 20);
	saveLaTex(all, "out.tex", sqrt(m_nbNodes), sqrt(m_nbNodes));
	saveHTML(evolutions, "evol.html", sqrt(m_nbNodes), sqrt(m_nbNodes), 2);

	for(unsigned int i(0); i < all.size(); i++)
	{
		delete all[i];
	}

	for(unsigned int i(0); i < evolutions.size(); i++)
	{
		delete evolutions[i];
	}
}

template<typename T>
void Graph<T>::complet(T *w)
{
	std::vector<Solution<T>*> all;

//TEST INDIVIDUEL

	bool *ind(new bool[m_nbNodes]);
	for(int i(0); i < m_nbNodes; i++)
	{
		ind[i] = (!((i-1)%15) || !((i-4)%15) || !((i-7)%15) || !((i-10)%15) || !((i-13)%15)) && i > 15 && i < 210;
	}

	for(int i(0); i < 15; i++)
	{
		ind[i+15] = true;
		if(i > 0 && i < 14)
		{/*
			ind[i+60] = true;
			ind[i+105] = true;
			ind[i+150] = true;*/
			ind[i+45] = true;
			ind[i+75] = true;
			ind[i+105] = true;
			ind[i+135] = true;
			ind[i+165] = true;
		}
		ind[i+195] = true;
	}

	Solution<T> *s(new Solution<T>(*this, ind));
	T tmp = s->valuation(*this, w);

	std::cout << s->getCost() << ";" << s->getAccess() << ";" << tmp << std::endl;
	all.push_back(s);
//*/
//FIN TEST INDIVIDUEL
/*

	bool *sol(new bool[m_nbNodes]);
	for(int i(0); i < m_nbNodes; i++)
	{
		sol[i] = false;
	}

	Solution<T> *max(nullptr);
	Solution<T> *maxV(nullptr);

	unsigned long long end(pow((float)2, m_nbNodes));
	std::cout << end << " " << m_nbNodes << std::endl;
	for(unsigned long long i(0); i < end; i++)
	{
		if(!(i%1000))
			printf("%llu/%llu;%lu  \r", i, end, all.size());
		for(int j(0); j < m_nbNodes; j++)
		{
			sol[j] = !sol[j];
			if(sol[j])
				break;
		}

		bool *tmp(new bool[m_nbNodes]);
		for(int j(0); j < m_nbNodes; j++)
		{
			tmp[j] = sol[j];
		}

		Solution<T> *s(new Solution<T>(*this, tmp));
		s->valuation(*this, w);

		if(max == nullptr || max->getScore() < s->getScore())
		{
			delete max;
			max = new Solution<T>(*this, s->getSolution());
			max->valuation(*this, w);
		}

		if(s->is_sol())
		{
			if(maxV == nullptr || max->getScore() < s->getScore())
			{
				delete maxV;
				maxV = new Solution<T>(*this, s->getSolution());
				maxV->valuation(*this, w);
			}

			if(*s <= all)
			{
				bool ajouter(true);
				for(int j(all.size()-1); j >= 0; j--)
				{
					if(*s == *all[j])
					{
						ajouter = false;
						delete s;
						break;
					}
					else if(*s << *all[j])
					{
						delete all[j];
						all.erase(all.begin()+j);
					}
				}

				if(ajouter)
					all.push_back(s);
			}
			else
			{
				delete s;
			}
		}
		else
		{
			delete s;
		}
	}

	all.push_back(max);
	all.push_back(maxV);//*/

	save(all, "outComplet");
	saveHTML(all, "outComplet.html", sqrt(m_nbNodes), sqrt(m_nbNodes), 20);
	//saveHTML(all, "outComplet.html", 4, 7);

	for(unsigned int i(0); i < all.size(); i++)
	{
		delete all[i];
	}
}

template<typename T>
void Graph<T>::las_vegas(int nbIt, int seed)
{
	std::vector<Solution<T>*> all;
	T w[] = {0,0,0,0};

	for(int i(0); i < nbIt; i++)
	{
		srand(seed+i);

		Solution<T> *current(new Solution<T>(*this));
		current->valuation(*this, w);

		add_sol(current, all);
	}


	saveHTML(all, "outLV.html", sqrt(m_nbNodes), sqrt(m_nbNodes), 20);
}

template<typename T>
void Graph<T>::save(const std::vector<Solution<T>*>& all, std::string path)
{
	std::ofstream o(path.c_str(), std::ios::out | std::ios::trunc);

	for(unsigned int i(0); i < all.size(); i++)
	{
		for(int j(0); j < m_nbNodes; j++)
		{
			o << all[i]->getSolution()[j];
		}
		o << std::endl;//*/
		o << all[i]->getCost() << ";" << all[i]->getAccess() << std::endl;
	}

	std::cout << std::endl << "Save ! " << std::endl;
}

template<typename T>
bool comp(Solution<T>* s1, Solution<T>* s2)
{
	return s1->getCost() < s2->getCost();
}

template<typename T>
void Graph<T>::saveHTML(std::vector<Solution<T>*>& all, std::string path, int x, int y, int X)
{
	//std::sort(all.begin(), all.end(), comp<T>);
	std::ofstream o(path.c_str(), std::ios::out | std::ios::trunc);

	o << "<table><tr>" << std::endl;

	for(unsigned int i(0); i < all.size(); i++)
	{
		if(i%X == 0)
			o << "</tr><tr>" << std::endl;
		o << "<td><table border=\"1\">" << std::endl;
		for(int j(0); j < x; j++)
		{
			o << "<tr>" << std::endl;
			for(int k(0); k < y; k++)
			{
				if(all[i]->getSolution()[j*y+k])
					o << "<td bgcolor=\"#000\">1</td>" << std::endl;
				else
					o << "<td><font color=\"white\">0</font></td>" << std::endl;
			}
			o << "</tr>" << std::endl;
		}
		o << "<tr><td colspan=\"" << y << "\">" << all[i]->getCost() << ";" << all[i]->getAccess() << ";" << all[i]->getScore() << "</td></tr>" << std::endl;
		o << "</table></td>" << std::endl;
	}

	o << "</tr></table>" << std::endl;

	std::cout << std::endl << "Save in HTML! " << std::endl;
}

template<typename T>
void Graph<T>::saveLaTex(std::vector<Solution<T>*>& all, std::string path, int x, int y)
{
	std::ofstream o(path.c_str(), std::ios::out | std::ios::trunc);

	o << "\\begin{center}" << std::endl;

	for(unsigned int i(0); i < all.size(); i++)
	{
		o << "\\begin{tikzpicture}" << std::endl << std::endl;

		o << "\\draw[step=1cm,black,very thin] (0,0) grid (" << x << "," << y << ");" << std::endl;

		for(int j(0); j < x; j++)
		{
			for(int k(0); k < y; k++)
			{
				if(all[i]->getSolution()[j*y+k])
				{
					o << "\\fill[black] (" << j << "," << k << ") rectangle (" << j+1 << "," << k+1 << ");" << std::endl;
				}
			}
		}

		o << std::endl << "\\end{tikzpicture}" << std::endl;
	}

	o << std::endl << "\\end{center}" << std::endl;

	std::cout << std::endl << "Save in LaTex! " << std::endl;
}

template<typename T>
int Graph<T>::getNbNodes() const
{
	return m_nbNodes;
}

template<typename T>
int Graph<T>::getCostAll() const
{
	return m_costAll;
}

template<typename T>
const Matrix<T>& Graph<T>::getMatrix0() const
{
	return m_mat0;
}

template<typename T>
T Graph<T>::getCost(int i) const
{
	return m_costs[i];
}

template<typename T>
T Graph<T>::path_length(int a, int b, const Solution<T>& s, const Matrix<T>& m) const
{
	bool *sol(s.getSolution());

	if(!sol[a] && !sol[b])
	{
		const std::vector<int>& roads(s.getRoads());

		int anb(a*m_nbNodes);
		int bnb(b*m_nbNodes);

		T path(std::numeric_limits<T>::infinity());

		for(unsigned int i(0); i < roads.size(); i++)
		{
			if(m_mat0[anb+roads[i]] != std::numeric_limits<T>::infinity())
			{
				for(unsigned int j(0); j < roads.size(); j++)
				{
					if(m_mat0[bnb+roads[j]] != std::numeric_limits<T>::infinity())
					{
						T tmp(m(roads[i], roads[j]));
						path = (path > tmp) ? tmp : path;
					}
				}
			}
		}

		return path / (m_mat[a*m_nbNodes+b]);
	}

	//return(sol[a] && sol[b]) ? m(a,b)/(m_mat[a*m_nbNodes+b]) : 0;

	return 0;
}

template<typename T>
int Graph<T>::access(bool *sol) const
{
	int acc(0);

	for(int i(0); i < m_nbNodes; i++)
	{
		if(!sol[i])
		{
			int tmp(0);
			for(int j(0); j < m_nbNodes; j++)
			{
				tmp += sol[j] && (m_mat0[i*m_nbNodes+j] != std::numeric_limits<T>::infinity());
			}
			acc += tmp != 0;
		}
	}

	return acc;
}




template class Graph<float>;








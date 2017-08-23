#include "solution.hpp"
#include "graph.hpp"

template<typename T>
Solution<T>::Solution() : m_sol(nullptr), m_roads(), m_cost(0), m_access(0), m_PLPCC(1), m_size(0), m_isSol(false), m_score(0.)
{
	
}

template<typename T>
Solution<T>::Solution(const Graph<T>& g, bool *sol) : m_sol(new bool[g.getNbNodes()]), m_roads(), m_cost(0), m_access(1), m_PLPCC(1), m_size(g.getNbNodes()), m_isSol(false), m_score(0.)
{
	for(int i(0); i < m_size; i++)
	{
		m_sol[i] = sol[i];
		if(m_sol[i])
		{
			m_roads.push_back(i);
		}
	}
}

template<typename T>
Solution<T>::Solution(const Graph<T>& g) : m_sol(new bool[g.getNbNodes()]), m_roads(), m_cost(0), m_access(1), m_PLPCC(1), m_size(g.getNbNodes()), m_isSol(false), m_score(0.)
{//*
	int p(rand()%(m_size+1));
	for(int i(0); i < m_size; i++)
	{
		m_sol[i] = (rand()%(m_size+1)) <= p;
		if(m_sol[i])
		{
			m_roads.push_back(i);
		}
	}//*/
/*
	for(int i(0); i < m_size; i++)
	{
		m_sol[i] = true;
		m_roads.push_back(i);
	}

	//*/
}

template<typename T>
Solution<T>::Solution(const Graph<T>& g, const Solution<T>& current, int change) : m_sol(new bool[g.getNbNodes()]), m_roads(), m_cost(0), m_access(1), m_PLPCC(1), m_size(g.getNbNodes()), m_isSol(false), m_score(0.)
{
	if(current.m_access == std::numeric_limits<T>::infinity() && current.m_sol[change])
	{
		for(int i(0); i < m_size; i++)
		{
			m_sol[i] = current.m_sol[i];

			if(m_sol[i])
			{
				m_roads.push_back(i);
			}
		}
	}
	else
	{
		for(int i(0); i < m_size; i++)
		{
			m_sol[i] = (i == change) ? !current.m_sol[i] : current.m_sol[i];

			if(m_sol[i])
			{
				m_roads.push_back(i);
			}
		}
	}
}

template<typename T>
Solution<T>::Solution(const Graph<T>& g, const Solution<T>& current, int* change, int size) : m_sol(new bool[g.getNbNodes()]), m_roads(), m_cost(0), m_access(1), m_PLPCC(1), m_size(g.getNbNodes()), m_isSol(false), m_score(0.)
{
	if(current.m_access == std::numeric_limits<T>::infinity())
	{
		for(int i(0); i < m_size; i++)
		{
			m_sol[i] = current.m_sol[i];

			std::find(change, change+size, i);

			if(std::find(change, change+size, i) != change+size && !m_sol[i])
				m_sol[i] = !m_sol[i];

			if(m_sol[i])
			{
				m_roads.push_back(i);
			}
		}
	}
	else
	{
		for(int i(0); i < m_size; i++)
		{
			m_sol[i] = (std::find(change, change+size, i) != change+size) ? !current.m_sol[i] : current.m_sol[i];

			if(m_sol[i])
			{
				m_roads.push_back(i);
			}
		}
	}
}

template<typename T>
Solution<T>::~Solution()
{
	delete[] m_sol;
}

template<typename T>
bool* Solution<T>::getSolution() const
{
	return m_sol;
}

template<typename T>
const std::vector<int>& Solution<T>::getRoads() const
{
	return m_roads;
}

template<typename T>
T Solution<T>::getCost() const
{
	return m_cost;
}

template<typename T>
T Solution<T>::getAccess() const
{
	return m_access;
}

template<typename T>
T Solution<T>::getScore() const
{
	return m_score;
}

template<typename T>
T Solution<T>::valuation(const Graph<T>& g, T *w)
{
	// Cout de la solution
	for(int i(0); i < m_size; i++)
	{
		m_cost += g.getCost(i) * m_sol[i];
	}

	//Accessibilité de la solution
	Matrix<T> m(g.getMatrix0(), m_sol);
	m.floyd_warshall();

	T moy(0.);
	int nb(0);

	for(int i(0); i < m_size; i++)
	{
		for(int j(i+1); j < m_size; j++)
		{
			T tmp(g.path_length(i, j, *this, m));
			moy += tmp;
			nb++;

			if(tmp > m_access)
			{
				m_access = tmp;
				m_PLPCC = 1;
			}
			else if(tmp == m_access)
			{
				m_PLPCC++;
			}
		}
	}

	//score mono de la solution
	moy /= nb;
	moy = (moy < 1.) ? 1. : moy;

	int acc(g.access(m_sol));

	int comp(1);
	for(unsigned int i(0); i < m_roads.size(); i++)
	{
		for(unsigned int j(0); j < m_roads.size(); j++)
		{
			comp += m(m_roads[i], m_roads[j]) == std::numeric_limits<T>::infinity();
		}
	}


	T c1((1+acc) / (T)(1+m_size-m_roads.size()));
	T c2(1. / (T)comp);
	T o1((g.getCostAll() - m_cost) / g.getCostAll());
	T o2;

	o2 = (c2 != 1.) ? 0. : 1./sqrt(m_access);

	m_isSol = c1 == 1. && c2 == 1.;
	m_score = w[0] * c1*c1 + w[1] * c2 + w[2] * o1 + w[3] * o2;

	return m_score;
}

template<typename T>
bool Solution<T>::is_sol() const
{
	return m_isSol;
}

template<typename T>
bool Solution<T>::operator==(const bool* s) const
{
	for(int i(0); i < m_size; i++)
	{
		if(s[i] != m_sol[i])
		{
			return false;
		}
	}
	return true;
}

template<typename T>
bool Solution<T>::operator==(const Solution<T>& s) const
{
	return *this == s.getSolution();
}


template<typename T>
bool Solution<T>::operator==(const std::vector<Solution*>& s) const
{
	for(unsigned int i(0); i < s.size(); i++)
	{
		if(*this == s[i]->m_sol)
		{
			return true;
		}
	}
	return false;
}

template<typename T>
bool Solution<T>::operator<(const Solution<T>& s) const
{
	return m_cost < s.m_cost || m_access < s.m_access;
}

template<typename T>
bool Solution<T>::operator>(const Solution& s) const
{
	return m_cost > s.m_cost || m_access > s.m_access;
}

template<typename T>
bool Solution<T>::operator<(const std::vector<Solution*>& s) const
{
	for(unsigned int i(0); i < s.size(); i++)
	{
		if(!(*this < *s[i]))
		{
			return false;
		}
	}
	return true;
}

template<typename T>
bool Solution<T>::operator>(const std::vector<Solution*>& s) const
{
	for(unsigned int i(0); i < s.size(); i++)
	{
		if(!(*this > *s[i]))
		{
			return false;
		}
	}
	return true;
}

template<typename T>
bool Solution<T>::operator<=(const Solution<T>& s) const
{
	return (m_cost < s.m_cost || m_access < s.m_access) || (m_cost == s.m_cost && m_access == s.m_access && m_PLPCC <= s.m_PLPCC);
}

template<typename T>
bool Solution<T>::operator>=(const Solution& s) const
{
	return (m_cost > s.m_cost || m_access > s.m_access) || (m_cost == s.m_cost && m_access == s.m_access && m_PLPCC >= s.m_PLPCC);
}

template<typename T>
bool Solution<T>::operator<=(const std::vector<Solution*>& s) const
{
	for(unsigned int i(0); i < s.size(); i++)
	{
		if(!(*this <= *s[i]))
		{
			return false;
		}
	}
	return true;
}

template<typename T>
bool Solution<T>::operator>=(const std::vector<Solution*>& s) const
{
	for(unsigned int i(0); i < s.size(); i++)
	{
		if(!(*this >= *s[i]))
		{
			return false;
		}
	}
	return true;
}

template<typename T>
bool Solution<T>::operator<<(const Solution& s) const
{
	//return (m_cost <= s.m_cost && m_access == s.m_access) || (m_cost == s.m_cost && m_access <= s.m_access) || (m_cost == s.m_cost && m_access == s.m_access && m_PLPCC <= s.m_PLPCC);
	return m_cost <= s.m_cost && m_access <= s.m_access && !(m_cost == s.m_cost && m_access == s.m_access && m_PLPCC >= s.m_PLPCC);
}

template<typename T>
bool Solution<T>::operator>>(const Solution& s) const
{
	return m_cost >= s.m_cost && m_access >= s.m_access && !(m_cost == s.m_cost && m_access == s.m_access && m_PLPCC <= s.m_PLPCC);
}

template class Solution<float>;






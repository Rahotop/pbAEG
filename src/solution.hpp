#ifndef SOLUTION_HPP_INCLUDED
#define SOLUTION_HPP_INCLUDED

#include <algorithm>
#include <vector>
#include <cmath>
#include "matrix.hpp"
template<typename N>
class Graph;

template<typename T>
class Solution
{
	public:

	Solution();
	Solution(const Graph<T>& g, bool *sol);
	Solution(const Graph<T>& g);
	Solution(const Graph<T>& g, const Solution<T>& current, int change);
	Solution(const Graph<T>& g, const Solution<T>& current, int *change, int size);
	~Solution();

	bool* getSolution() const;
	const std::vector<int>& getRoads() const;
	T getCost() const;
	T getAccess() const;
	T getScore() const;

	T valuation(const Graph<T>& g, T *w);
	bool is_sol() const;

	bool operator==(const bool* s) const;
	bool operator==(const Solution<T>& s) const;
	bool operator==(const std::vector<Solution<T>*>& s) const;
	bool operator<(const Solution<T>& s) const;
	bool operator>(const Solution<T>& s) const;
	bool operator<(const std::vector<Solution<T>*>& s) const;
	bool operator>(const std::vector<Solution<T>*>& s) const;
	bool operator<=(const Solution<T>& s) const;
	bool operator>=(const Solution<T>& s) const;
	bool operator<=(const std::vector<Solution<T>*>& s) const;
	bool operator>=(const std::vector<Solution<T>*>& s) const;
	bool operator<<(const Solution<T>& s) const;
	bool operator>>(const Solution<T>& s) const;

	private:

	bool* m_sol;
	std::vector<int> m_roads;
	T m_cost;
	T m_access;
	int m_PLPCC;
	int m_size;
	bool m_isSol;
	T m_score;
};

#endif
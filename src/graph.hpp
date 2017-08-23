#ifndef GRAPH_HPP_INCLUDED
#define GRAPH_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <cstdlib>
#include <cmath>
#include "matrix.hpp"
#include "solution.hpp"

template<typename T>
class Graph
{
	public:

	Graph();
	Graph(std::string path);
	~Graph();

	void local_search(int nbIt, T *w, int seed);
	void local_search(int nbIt, T *w, int seed, T tau0, T alpha);
	void complet(T *w);
	void las_vegas(int nbIt, int seed);
	void save(const std::vector<Solution<T>*>& all, std::string path);
	void saveHTML(std::vector<Solution<T>*>& all, std::string path, int x, int y, int X);
	void saveLaTex(std::vector<Solution<T>*>& all, std::string path, int x, int y);

	int getNbNodes() const;
	int getCostAll() const;
	const Matrix<T>& getMatrix0() const;

	T getCost(int i) const;
	T path_length(int a, int b, const Solution<T>& s, const Matrix<T>& m) const;
	int access(bool *sol) const;

	private:

	int m_nbNodes;
	T *m_costs;
	T m_costAll;

	Matrix<T> m_mat;
	Matrix<T> m_mat0;
};

#endif






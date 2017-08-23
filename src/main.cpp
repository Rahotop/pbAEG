#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "graph.hpp"

int main(int argc, char *argv[])
{
	int nbIt(10);
	srand(time(nullptr));
	int seed(rand());
	int method(0);
	float alpha(0.9), tau(50.);
	float w[4] = {1000.,2000.,1.,1.};
	for(int i(1); i < argc; i++)
	{
		if(std::string(argv[i]) == "-it")
		{
			nbIt = std::stoi(argv[i+1]);
		}
		else if(std::string(argv[i]) == "-seed")
		{
			seed = std::stoi(argv[i+1]);
			srand(seed);
		}
		else if(std::string(argv[i]) == "-exp")
		{
			w[0] = std::stof(argv[i+1]);
		}
		else if(std::string(argv[i]) == "-conn")
		{
			w[1] = std::stof(argv[i+1]);
		}
		else if(std::string(argv[i]) == "-cost")
		{
			w[2] = std::stof(argv[i+1]);
		}
		else if(std::string(argv[i]) == "-acc")
		{
			w[3] = std::stof(argv[i+1]);
		}
		else if(std::string(argv[i]) == "-ls")
		{
			method = 0;
		}
		else if(std::string(argv[i]) == "-recuit")
		{
			method = 1;
			tau = std::stof(argv[i+1]);
			alpha = std::stof(argv[i+2]);
		}
		else if(std::string(argv[i]) == "-comp")
		{
			method = 2;
		}
		else if(std::string(argv[i]) == "-LV")
		{
			method = 3;
		}
	}


	std::cout << "Start read file" << std::endl;

	Graph<float> g("graphs/graph.txt");

	std::cout << "End Initialisation" << std::endl;

	switch(method)
	{
		case 0:
			g.local_search(nbIt, w, seed);
			break;
		case 1:
			g.local_search(nbIt, w, seed, tau, alpha);
			break;
		case 2:
			g.complet(w);
			break;
		case 3:
			g.las_vegas(nbIt, seed);
			break;
	}
	return 0;
}

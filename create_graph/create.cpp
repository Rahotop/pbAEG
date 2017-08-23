#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

int main(int argc, char *argv[])
{
	srand(time(nullptr));

	std::string path("graph.txt");
	int nodes(100);
	bool grille(false);
	bool random(false);
	int density(50);
	int maxW(1000);
	for(int i(1); i < argc; i++)
	{
		if(std::string(argv[i]) == "-o")
		{
			path = argv[i+1];
		}
		else if(std::string(argv[i]) == "-g")
		{
			grille = true;
		}
		else if(std::string(argv[i]) == "-gr")
		{
			grille = true;
			random = true;
		}
		else if(std::string(argv[i]) == "-n")
		{
			nodes = std::stoi(argv[i+1]);
		}
		else if(std::string(argv[i]) == "-w")
		{
			maxW = std::stoi(argv[i+1]);
		}
		else if(std::string(argv[i]) == "-d")
		{
			density = std::stoi(argv[i+1]);
		}
		else if(std::string(argv[i]) == "-h")
		{
			std::cout << "-o path -- To specify the path of the output" << std::endl;
			std::cout << "-g      -- Will create a lattice square graph with all weight set to 1 (-n will be the side of it)" << std::endl;
			std::cout << "-gr     -- Will create a lattice square graph with weights set randomly (-n will be the side of it)" << std::endl;
			std::cout << "-n int  -- To specify the number of nodes to create" << std::endl;
			std::cout << "-w int  -- To specify the maximum weight of a transition" << std::endl;
			std::cout << "-d int  -- To specify the density (in percent) of the graph" << std::endl;
			std::cout << "The final density of the graph is not guaranteed to be -d" << std::endl;
		}
	}

	std::ofstream out(path.c_str());

	if(out.is_open())
	{

		out << "c FILE : " << path << std::endl;
		out << "c " << std::endl;
		if(grille)
		{
			out << "c NODES : " << nodes*nodes << std::endl;
			out << "c LATTICE SQUARE GRAPH" << std::endl;
		}
		else
		{
			out << "c NODES : " << nodes << std::endl;
			out << "c DENSITY : " << density << std::endl;
		}
		out << "c " << std::endl;
		out << "c " << std::endl;


		if(!grille)
		{
			out << "node " << nodes << std::endl;
			for(int i(0); i < nodes; i++)
			{
				out << rand()%maxW << " ";
			}
		}
		else
		{
			out << "node " << nodes*nodes << std::endl;
			for(int i(0); i < nodes*nodes; i++)
			{
				if(random)
					out << rand()%maxW << " ";
				else
					out << "1 ";
			}
		}

		out << std::endl;

		if(!grille)
		{
			for(int i(0); i < nodes; i++)
			{
				for(int j(i+1); j < nodes; j++)
				{
					if((rand()%100) < density)
						out << "t " << i << " " << j << " " << rand()%maxW << std::endl;
				}
			}
		}
		else
		{
			for(int i(0); i < nodes; i++)
			{
				for(int j(0); j < nodes; j++)
				{
					int tmp(i*nodes+j);
					if(j+1 < nodes)
					{
						if(random)
							out << "t " << tmp << " " << tmp+1 << " " << rand()%maxW << std::endl;
						else
							out << "t " << tmp << " " << tmp+1 << " 1" << std::endl;
					}
					if(i+1 < nodes)
					{
						if(random)
							out << "t " << tmp << " " << tmp+nodes << " " << rand()%maxW << std::endl;
						else
							out << "t " << tmp << " " << tmp+nodes << " 1" << std::endl;
					}
				}
			}
		}
	}
	else
	{
		std::cout << "error : can't open output file." << std::endl;
	}

	return 0;
}

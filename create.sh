g++ -std=c++11 -o ./create_graph/create ./create_graph/*.cpp
echo `./create_graph/create -o ./graphs/graph.txt $* `

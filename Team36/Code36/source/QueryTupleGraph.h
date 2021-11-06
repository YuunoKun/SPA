#pragma once

#include <string>
#include "QueryTupleNode.h"

class QueryTupleGraph {
public:

	QueryTupleGraph(int V);
	void addEdge(int v, int w);
	void connectedComponents(std::list<std::list<std::pair<Entity, Entity>>>&);

private:

	int V;
	std::list<int>* adj;
	void DFSUtil(int, bool visited[], std::list<int>&);
};
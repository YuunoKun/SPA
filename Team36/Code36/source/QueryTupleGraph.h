#pragma once

#include <string>
#include "QueryTupleNode.h"

class QueryTupleGraph {
public:

	QueryTupleGraph(int V);
	void addEdge(int v, int w);
	std::list<std::list<std::pair<int, int>>> connectedComponents();

private:

	int V;
	std::list<int>* adj;
	std::list<int> DFSUtil(int, bool visited[]);
};
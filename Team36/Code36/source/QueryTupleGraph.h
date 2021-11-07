#pragma once

#include <string>
#include "QueryTupleNode.h"

class QueryTupleGraph {
public:

	QueryTupleGraph(int V);
	void addEdge(int v, int w);
	std::list<std::list<int>> connectedComponents();

private:

	int V;
	std::list<int>* adj;
	void DFSUtil(int, bool visited[], std::list<int>&);
};
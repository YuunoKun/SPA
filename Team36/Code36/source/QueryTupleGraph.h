#pragma once

#include <string>
#include "QueryTupleNode.h"

class QueryTupleGraph {
public:

	QueryTupleGraph(int v, std::list<int>*);
	void addEdge(int v, int w);
	void connectedComponents();

private:

	int V;
	std::list<int>* adj;
	void DFSUtil(int, bool visited[]);
};
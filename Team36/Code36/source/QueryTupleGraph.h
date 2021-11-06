#pragma once

#include <string>
#include "QueryTupleNode.h"

class QueryTupleGraph {
public:

	QueryTupleGraph(Entity, std::vector<Entity>*);

private:

	Entity entity;
	std::vector<Entity>* neighbours;
	void DFSUtil(Entity, bool);
};
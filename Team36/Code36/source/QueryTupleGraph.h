#pragma once

#include <string>
#include <vector>
#include "QueryTupleNode.h"

class QueryTupleGraph {
public:

	QueryTupleGraph(QueryTupleNode, std::vector<QueryTupleNode>);

private:

	QueryTupleNode node;
	std::vector<QueryTupleNode> neighbours;
};
#include <vector>
#include "QueryTupleGraph.h"
#include "QueryTupleNode.h"

QueryTupleGraph::QueryTupleGraph(QueryTupleNode node, std::vector<QueryTupleNode> neighbours) {
	this->node = node;
	this->neighbours = neighbours;
}
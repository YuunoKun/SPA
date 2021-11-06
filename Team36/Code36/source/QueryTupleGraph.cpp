#include <vector>
#include "QueryTupleGraph.h"
#include "QueryTupleNode.h"

QueryTupleGraph::QueryTupleGraph(Entity entity, std::vector<Entity>* neighbours) {
	this->entity = entity;
	this->neighbours = neighbours;
}
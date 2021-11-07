#include "QueryTupleNode.h"
#include "Entity.h"

QueryTupleNode::QueryTupleNode() {
}

QueryTupleNode::QueryTupleNode(Entity entity) {
	this->entity = entity;
}
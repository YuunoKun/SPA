#include "QueryTupleNode.h"
#include "Entity.h"

QueryTupleNode::QueryTupleNode() {
}

QueryTupleNode::QueryTupleNode(Entity entity, bool selected) {
	this->entity = entity;
	this->isSelected = selected;
}

QueryTupleNode::QueryTupleNode(Entity entity) {
	this->entity = entity;
}
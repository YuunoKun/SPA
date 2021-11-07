#pragma once

#include <string>

#include "Entity.h"

class QueryTupleNode {
public:

	//bool operator==(const Pattern&) const;

	QueryTupleNode(Entity);
	QueryTupleNode();

private:
	Entity entity;
};
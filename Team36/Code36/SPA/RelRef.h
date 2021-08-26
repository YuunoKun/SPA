#pragma once

#include <string>

#include "Entity.h"

using namespace std;

enum RelType {
	modifiesS,
	usesS,
	parent,
	parentT,
	follows,
	followsT,
};

class RelRef {
public:
	RelRef(RelType, Entity, Entity);

	RelType getType();
	Entity getFirstClause();
	Entity getSecondClause();

private:

	//Member Variable
	RelType rel_type;
	Entity first_clause;
	Entity second_clause;
};
#pragma once

#include <string>

#include "Entity.h"

using namespace std;

enum RelType {
	MODIFIES_S,
	MODIFIES_P,
	USES_S,
	USE_P,
	PARENT,
	PARENT_T,
	FOLLOWS,
	FOLLOWS_T
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
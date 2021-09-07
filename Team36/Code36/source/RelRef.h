#pragma once

#include <string>
#include <vector>

#include "Entity.h"

enum RelType {
	MODIFIES_S,
	MODIFIES_P,
	USES_S,
	USES_P,
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

	bool operator==(const RelRef&) const;

private:

	//Member Variable
	RelType rel_type;
	Entity first_clause;
	Entity second_clause;
};
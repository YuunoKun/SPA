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
	FOLLOWS_T,
	CALLS,
	CALLS_T,
	NEXT,
	NEXT_T,
	AFFECT,
	AFFECT_T,
	WITH
};

class RelRef {
public:
	RelRef(RelType, Entity, Entity);
	RelRef();

	RelType getType();
	Entity getFirstClause();
	Entity getSecondClause();

	bool operator==(const RelRef&) const;

private:
	RelType rel_type;
	Entity first_clause;
	Entity second_clause;
};
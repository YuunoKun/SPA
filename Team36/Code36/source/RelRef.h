#pragma once

#include <string>
#include <vector>

#include "Entity.h"

enum class RelType {
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
	RelRef(RelType, std::vector<Entity>);

	RelType getType();
	std::vector<Entity> getClauses();

private:

	//Member Variable
	RelType rel_type;
	std::vector<Entity> clauses;
};
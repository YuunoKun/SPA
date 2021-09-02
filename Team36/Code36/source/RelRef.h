#pragma once

#include <string>
#include <vector>

#include "Entity.h"

enum RelType {
	//MODIFIES_S,
	MODIFIES_P,
	//USES_S,
	USES_P,
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

	bool operator==(const RelRef&) const;

private:

	//Member Variable
	RelType rel_type;
	std::vector<Entity> clauses;
};
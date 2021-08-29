#pragma once

#include <string>
#include <vector>

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
	RelRef(RelType, vector<Entity>);

	RelType getType();
	vector<Entity> getClauses();

private:

	//Member Variable
	RelType rel_type;
	vector<Entity> clauses;
};
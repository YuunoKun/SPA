#include "RelRef.h"


RelRef::RelRef(RelType rel_type, vector<Entity> clauses) {
	this->rel_type = rel_type;
	this->clauses = clauses;
}

RelType RelRef::getType() {
	return rel_type;
}

vector<Entity> RelRef::getClauses() {
	return clauses;
}


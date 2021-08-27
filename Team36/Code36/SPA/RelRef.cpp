#include "RelRef.h"


RelRef::RelRef(RelType rel_type, Entity first_clause, Entity second_clause) {
	this->rel_type = rel_type;
	this->first_clause = first_clause;
	this->second_clause = second_clause;
}

RelType RelRef::getType() {
	return rel_type;
}

Entity RelRef::getFirstClause() {
	return first_clause;
}

Entity RelRef::getSecondClause() {
	return second_clause;
}

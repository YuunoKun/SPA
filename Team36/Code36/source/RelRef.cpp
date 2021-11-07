#include "RelRef.h"
#include <iostream>

RelRef::RelRef(RelType rel_type, Entity first_clause, Entity second_clause) {
	std::vector<Entity> clauses{ first_clause, second_clause };
	this->rel_type = rel_type;
	this->first_clause = first_clause;
	this->second_clause = second_clause;
}

RelRef::RelRef() {
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

bool RelRef::operator==(const RelRef& entity) const {
	return rel_type == entity.rel_type
		&& first_clause == entity.first_clause
		&& second_clause == entity.second_clause;
}
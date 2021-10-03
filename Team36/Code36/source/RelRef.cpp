#include "RelRef.h"
#include <iostream>

RelRef::RelRef(RelType rel_type, Entity first_clause, Entity second_clause) {
	// Checks that pattern types are valid
	std::vector<Entity> clauses{ first_clause, second_clause };
	// TODO: Add follows, followsT, parent, parentT after discussion for new type
	if (rel_type == USES_P) {
		for (unsigned int i = 0; i < clauses.size(); ++i) {
			if (clauses[i].getType() != static_cast<EntityType>(VARIABLE) &&
				clauses[i].getType() != static_cast<EntityType>(PROCEDURE) &&
				clauses[i].getType() != static_cast<EntityType>(WILD) &&
				clauses[i].getType() != static_cast<EntityType>(CONSTANT)) {
				throw std::invalid_argument("Clause Type is invalid for USES_P type");
			}
		}
	}

	if (rel_type == MODIFIES_P) {
		for (unsigned int i = 0; i < clauses.size(); ++i) {
			if (clauses[i].getType() != static_cast<EntityType>(VARIABLE) &&
				clauses[i].getType() != static_cast<EntityType>(PROCEDURE) &&
				clauses[i].getType() != static_cast<EntityType>(WILD) &&
				clauses[i].getType() != static_cast<EntityType>(CONSTANT)) {
				throw std::invalid_argument("Clause Type is invalid for MODIFIES_P type");
			}
		}
	}

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
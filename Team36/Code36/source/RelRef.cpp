#include "RelRef.h"
#include <iostream>

RelRef::RelRef(RelType rel_type, std::vector<Entity> clauses) {
	// Checks that pattern types are valid

	// TODO: Add follows, followsT, parent, parentT after discussion for new type
	if (rel_type == USES_P) {
		for (int i = 0; i < clauses.size(); ++i) {
			if (clauses[i].getType() != static_cast<EntityType>(ASSIGN) &&
				clauses[i].getType() != static_cast<EntityType>(VARIABLE) &&
				clauses[i].getType() != static_cast<EntityType>(PRINT) &&
				clauses[i].getType() != static_cast<EntityType>(IF) &&
				clauses[i].getType() != static_cast<EntityType>(WHILE) &&
				clauses[i].getType() != static_cast<EntityType>(PROCEDURE) &&
				clauses[i].getType() != static_cast<EntityType>(CALL) &&
				clauses[i].getType() != static_cast<EntityType>(CONSTANT)) {
				throw std::invalid_argument("Clause Type is invalid for USES_P type");
			}
		}
	}

	if (rel_type == MODIFIES_P) {
		for (int i = 0; i < clauses.size(); ++i) {
			if (clauses[i].getType() != static_cast<EntityType>(ASSIGN) &&
				clauses[i].getType() != static_cast<EntityType>(VARIABLE) &&
				clauses[i].getType() != static_cast<EntityType>(READ) &&
				clauses[i].getType() != static_cast<EntityType>(IF) &&
				clauses[i].getType() != static_cast<EntityType>(WHILE) &&
				clauses[i].getType() != static_cast<EntityType>(PROCEDURE) &&
				clauses[i].getType() != static_cast<EntityType>(CALL) &&
				clauses[i].getType() != static_cast<EntityType>(CONSTANT)) {
				throw std::invalid_argument("Clause Type is invalid for MODIFIES_P type");
			}
		}
	}

	this->rel_type = rel_type;
	this->clauses = clauses;
}

RelType RelRef::getType() {
	return rel_type;
}

std::vector<Entity> RelRef::getClauses() {
	return clauses;
}

bool RelRef::operator==(const RelRef& entity) const {
	return rel_type == entity.rel_type
		&& clauses == entity.clauses;
}
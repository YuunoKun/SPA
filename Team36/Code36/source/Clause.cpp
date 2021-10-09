#include "Clause.h"

Clause::Clause(RelRef rel) {
	this->rel = rel;
	this->type = RELATION;
}

Clause::Clause(Pattern pattern) {
	this->pattern = pattern;
	this->type = PATTERN;
}

ClauseType Clause::getType() {
	return type;
}

RelRef Clause::getRelation() {
	return rel;
}

Pattern Clause::getPattern() {
	return pattern;
}

bool Clause::operator==(const Clause& other) const {
	return type == other.type &&
		((type == RELATION && rel == other.rel) ||
		(type == PATTERN && pattern == other.pattern));
}



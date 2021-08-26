#include "Pattern.h"

Pattern::Pattern(Entity ent_ref, string expression, bool wild) {
	this->ent_ref = ent_ref;
	this->expression = expression;
	this->wild = wild;
}

Entity Pattern::getEntityRef() {
	return ent_ref;
}

string Pattern::getExpression() {
	return expression;
}

bool Pattern::isWild() {
	return wild;
}

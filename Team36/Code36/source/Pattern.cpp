#include "Pattern.h"

Pattern::Pattern(Entity ent_ref, TNode expression, bool is_wild) {
	this->ent_ref = ent_ref;
	this->expression = expression;
	this->is_wild = is_wild;
}

Entity Pattern::getEntityRef() {
	return ent_ref;
}

TNode Pattern::getExpression() {
	return expression;
}

bool Pattern::isWild() {
	return is_wild;
}
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

//TODO: compare expression when == TNode is updated
bool Pattern::operator==(const Pattern& pattern) const {
	return ent_ref == pattern.ent_ref
		//&& expression == pattern.expression
		&& is_wild == pattern.is_wild;
}

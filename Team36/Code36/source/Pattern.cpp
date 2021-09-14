#include "Pattern.h"
#include <iostream>

Pattern::Pattern(Entity pattern_type, Entity left_expression, expr expression, bool is_wild) {
	// Checks that pattern types are valid
	if (pattern_type.getType() != static_cast<EntityType>(ASSIGN)) {
		throw std::invalid_argument("Pattern Type is invalid");
	}

	if (left_expression.getType() != static_cast<EntityType>(VARIABLE) &&
		left_expression.getType() != static_cast<EntityType>(WILD)) {
		throw std::invalid_argument("Left Expression Type is invalid");
	}

	this->pattern_type = pattern_type;
	this->left_expression = left_expression;
	this->expression = expression;
	this->is_wild = is_wild;
}

Entity Pattern::getPatternType() {
	return pattern_type;
}

Entity Pattern::getLeftExpression() {
	return left_expression;
}

std::string Pattern::getExpression() {
	return expression;
}

bool Pattern::isWild() {
	return is_wild;
}

//TODO: compare expression when == TNode is updated
bool Pattern::operator==(const Pattern& pattern) const {
	return pattern_type == pattern.pattern_type
		&& left_expression == pattern.left_expression
		&& expression == pattern.expression
		&& is_wild == pattern.is_wild;
}
#include "Pattern.h"
#include <iostream>
#include "ExprParser.h"

Pattern::Pattern(Entity pattern_type, Entity left_expression, std::string expr, bool is_wild) {
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
	if (expr == "") {
		this->expression = nullptr;
	}else {
		this->expression = expr_parser.parse(expr);
	}
	this->is_wild = is_wild;
}

Pattern::Pattern() {
}

Pattern::~Pattern() {
	delete this->expression;
}

Entity Pattern::getPatternType() {
	return pattern_type;
}

Entity Pattern::getLeftExpression() {
	return left_expression;
}

expr* Pattern::getExpression() {
	return expression;
}

bool Pattern::isWild() {
	return is_wild;
}

//TODO: compare expression when == TNode is updated
bool Pattern::operator==(const Pattern& pattern) const {
	return pattern_type == pattern.pattern_type
		&& left_expression == pattern.left_expression
		&& ((!expression && !pattern.expression) || (expression)->equals(pattern.expression))
		&& is_wild == pattern.is_wild;
}
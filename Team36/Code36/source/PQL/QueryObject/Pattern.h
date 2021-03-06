#pragma once

#include <string>

#include "Entity.h"

class Pattern {
public:
	Pattern(Entity, Entity, std::string, bool);
	Pattern(Entity, Entity);
	Pattern();

	Entity getPatternType();
	Entity getLeftExpression();
	expr getExpression();
	bool isWild();

	bool operator==(const Pattern&) const;

private:
	Entity pattern_type;
	Entity left_expression;
	expr expression;
	bool is_wild;
};
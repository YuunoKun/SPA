#pragma once

#include <string>

#include "Entity.h"
#include "TNode.h"

class Pattern {
public:
	Pattern(Entity, Entity, std::string, bool);

	Entity getPatternType();
	Entity getLeftExpression();
	std::string getExpression();
	bool isWild();

	bool operator==(const Pattern&) const;

private:

	//Member Variable
	Entity pattern_type;
	Entity left_expression;
	std::string expression;
	bool is_wild;
};
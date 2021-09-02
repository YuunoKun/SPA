#pragma once

#include <string>

#include "Entity.h"
#include "TNode.h"

class Pattern {
public:
	Pattern(Entity, Entity, TNode, bool);

	Entity getPatternType();
	Entity getLeftExpression();
	TNode getExpression();
	bool isWild();

	bool operator==(const Pattern&) const;

private:

	//Member Variable
	Entity pattern_type;
	Entity left_expression;
	TNode expression;
	bool is_wild;
};
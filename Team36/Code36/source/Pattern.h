#pragma once

#include <string>

#include "Clause.h"
#include "Entity.h"
#include "TNode.h"

class Pattern : public Clause {
public:
	Pattern(Entity, Entity, expr, bool);

	Entity getPatternType();
	Entity getLeftExpression();
	expr getExpression();
	bool isWild();

	bool operator==(const Pattern&) const;

private:

	//Member Variable
	Entity pattern_type;
	Entity left_expression;
	expr expression;
	bool is_wild;
};
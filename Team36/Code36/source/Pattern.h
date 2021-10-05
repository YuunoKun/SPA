#pragma once

#include <string>

#include "Entity.h"
#include "Common.h"
#include "ExprParser.h"

class Pattern {
public:
	Pattern(Entity, Entity, std::string, bool);
	Pattern();
	Pattern(const Pattern&);
	~Pattern();

	Entity getPatternType();
	Entity getLeftExpression();
	expr* getExpression();
	bool isWild();

	bool operator==(const Pattern&) const;
	Pattern& operator=(const Pattern&);

private:
	ExprParser expr_parser;
	//Member Variable
	Entity pattern_type;
	Entity left_expression;
	expr* expression{ nullptr };
	bool is_wild;
};
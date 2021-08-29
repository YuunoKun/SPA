#pragma once

#include <string>

#include "Entity.h"
#include "TNode.h"

class Pattern {
public:
	Pattern(Entity, TNode, bool);

	Entity getEntityRef();
	TNode getExpression();
	bool isWild();

private:

	//Member Variable
	Entity ent_ref;
	TNode expression;
	bool is_wild;
};
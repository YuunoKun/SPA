#pragma once

#include <string>

#include "Entity.h"

using namespace std;



class Pattern {
public:
	Pattern(Entity, string, bool);

	Entity getEntityRef();
	string getExpression();
	bool isWild();

private:

	//Member Variable
	Entity ent_ref;
	string expression;
	bool wild;
};
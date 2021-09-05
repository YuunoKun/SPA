#pragma once

#include "Entity.h"
#include "ResultTable.h"
#include "PKBAdapter.h"

class RelationEvaluatorInterface {
public:

	//Handle both wild : e.g Relation(_, _)
	virtual bool haveRelation() = 0;

	//Handle both constant : e.g Relation(1, 2)
	virtual bool isRelation(Entity, Entity) = 0;

	//Handle left constant, right wild: e.g Relation(1, _)
	virtual bool haveRelationAtRight(Entity) = 0;

	//Handle right wild, left constant: e.g Relation(_, 1)
	virtual bool haveRelationAtLeft(Entity) = 0;

	//Handle both declartion : e.g Relation(a, b)
	virtual ResultTable getRelations(Entity, Entity) = 0;

	//Handle left constant, right declartion: e.g Relation(_, a)
	virtual ResultTable getRightRelations(Entity) = 0;

	//Handle right declartion, left constant: e.g Relation(a, _)
	virtual ResultTable getLeftRelations(Entity) = 0;

	//Handle left constant, right declartion: e.g Relation(1, a)
	virtual ResultTable getRelationMatchLeft(Entity, Entity) = 0;

	//Handle right declartion, left constant: e.g Relation(a, 1)
	virtual ResultTable getRelationMatchRight(Entity, Entity) = 0;


protected:
	PKBAdapter pkb;
};
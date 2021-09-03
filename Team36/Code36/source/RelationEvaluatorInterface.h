#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"

class RelationEvaluator {
public:
	//Handle both wild : e.g Relation(_, _)
	virtual bool haveRelation();

	//Handle both constant : e.g Relation(1, 2)
	virtual bool isRelation(Entity, Entity);

	//Handle left constant, right wild: e.g Relation(1, _)
	virtual bool haveRelationAtRight(Entity);

	//Handle right wild, left constant: e.g Relation(_, 1)
	virtual bool haveRelationAtLeft(Entity);

	//Handle both declartion : e.g Relation(a, b)
	virtual std::vector<std::vector<std::string>> getRelation(Entity, Entity);

	//Handle left constant, right declartion: e.g Relation(_, a)
	virtual ResultTable getAllRightRelation(Entity, Entity);

	//Handle right declartion, left constant: e.g Relation(a, _)
	virtual ResultTable getAllLeftRelation(Entity, Entity);

	//Handle left constant, right declartion: e.g Relation(1, a)
	virtual ResultTable getRelationMatchLeft(Entity, Entity);

	//Handle right declartion, left constant: e.g Relation(a, 1)
	virtual ResultTable getRelationMatchRight(Entity, Entity);



private:
	PKBAdapter pkb;
};
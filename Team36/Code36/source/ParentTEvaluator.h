#pragma once


#include "RelationEvaluatorInterface.h"
#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"

class ParentTEvaluator : public RelationEvaluatorInterface {
public:
	//Handle both wild : e.g Relation(_, _)
	bool haveRelation() override;

	//Handle both constant : e.g Relation(1, 2)
	bool isRelation(Entity, Entity) override;

	//Handle left constant, right wild: e.g Relation(1, _)
	bool haveRelationAtRight(Entity) override;

	//Handle right wild, left constant: e.g Relation(_, 1)
	bool haveRelationAtLeft(Entity) override;

	//Handle both declartion : e.g Relation(a, b)
	ResultTable getRelations(Entity, Entity) override;

	//Handle left constant, right declartion: e.g Relation(_, a)
	ResultTable getRightRelations(Entity) override;

	//Handle right declartion, left constant: e.g Relation(a, _)
	ResultTable getLeftRelations(Entity) override;

	//Handle left constant, right declartion: e.g Relation(1, a)
	ResultTable getRelationMatchLeft(Entity, Entity) override;

	//Handle right declartion, left constant: e.g Relation(a, 1)
	ResultTable getRelationMatchRight(Entity, Entity) override;


};
#pragma once


#include "RelationEvaluatorInterface.h"
#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"

class CallsEvaluator : public RelationEvaluatorInterface {
public:
	//Handle both wild : e.g Relation(_, _)
	bool evaluateWildAndWild() override;

	//Handle both constant : e.g Relation(1, 2)
	bool evaluateConstantAndConstant(Entity, Entity) override;

	//Handle left constant, right wild: e.g Relation(1, _)
	bool evaluateConstantAndWild(Entity) override;

	//Handle right wild, left constant: e.g Relation(_, 1)
	bool evaluateWildAndConstant(Entity) override;

	//Handle both declartion : e.g Relation(a, b)
	ResultTable evaluateSynonymAndSynonym(Entity, Entity) override;

	//Handle left constant, right declartion: e.g Relation(_, a)
	ResultTable evaluateWildAndSynonym(Entity) override;

	//Handle right declartion, left constant: e.g Relation(a, _)
	ResultTable evaluateSynonymAndWild(Entity) override;

	//Handle left constant, right declartion: e.g Relation(1, a)
	ResultTable evaluateConstantAndSynonym(Entity, Entity) override;

	//Handle right declartion, left constant: e.g Relation(a, 1)
	ResultTable evaluateSynonymAndConstant(Entity, Entity) override;
};
#pragma once


#include "RelationEvaluatorInterface.h"
#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"

class NextBipEvaluator : public RelationEvaluatorInterface {
public:
	bool evaluateWildAndWild() override;
	bool evaluateConstantAndConstant(Entity, Entity) override;
	bool evaluateConstantAndWild(Entity) override;
	bool evaluateWildAndConstant(Entity) override;
	ResultTable evaluateSynonymAndSynonym(Entity, Entity) override;
	ResultTable evaluateWildAndSynonym(Entity) override;
	ResultTable evaluateSynonymAndWild(Entity) override;
	ResultTable evaluateConstantAndSynonym(Entity, Entity) override;
	ResultTable evaluateSynonymAndConstant(Entity, Entity) override;
};
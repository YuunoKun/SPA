#pragma once

#include "PKB/PKBAdapter.h"
#include "PQL/QueryObject/Entity.h"
#include "PQL/QueryEvaluator/ResultTable.h"

class RelationEvaluatorInterface {
public:
	virtual bool evaluateWildAndWild() = 0;
	virtual bool evaluateConstantAndConstant(Entity, Entity) = 0;
	virtual bool evaluateConstantAndWild(Entity) = 0;
	virtual bool evaluateWildAndConstant(Entity) = 0;
	virtual ResultTable evaluateSynonymAndSynonym(Entity, Entity) = 0;
	virtual ResultTable evaluateWildAndSynonym(Entity) = 0;
	virtual ResultTable evaluateSynonymAndWild(Entity) = 0;
	virtual ResultTable evaluateConstantAndSynonym(Entity, Entity) = 0;
	virtual ResultTable evaluateSynonymAndConstant(Entity, Entity) = 0;
protected:
	PKBAdapter pkb;
};
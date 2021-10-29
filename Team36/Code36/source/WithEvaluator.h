#pragma once


#include "RelationEvaluatorInterface.h"
#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"

class WithEvaluator : public RelationEvaluatorInterface {
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

private:
	std::list<std::vector<std::string>> getEntity(Entity);
	std::list<std::vector<std::string>> getEntityAndSecondaryAttribute(Entity);


	std::list<std::string> getEntityWithAttribute(Entity, Entity);
	std::list<std::string> getEntityWithSecondaryAttribute(Entity);
};
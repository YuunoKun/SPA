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
	void getEntity(Entity&, std::list<std::vector<std::string>>&);
	void getEntityAndSecondaryAttribute(Entity&, std::list<std::vector<std::string>>&);


	void getEntityWithAttribute(Entity&, Entity&, std::list<std::vector<std::string>>&);
	void getEntityWithSecondaryAttribute(Entity&, std::list<std::vector<std::string>>&);
};
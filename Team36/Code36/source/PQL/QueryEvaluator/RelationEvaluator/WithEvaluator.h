#pragma once

#include "RelationEvaluatorInterface.h"

class WithEvaluator : public RelationEvaluatorInterface {
public:
	WithEvaluator();

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
	Entity string_dummy, num_dummy;

	ResultTable getEntity(Entity&);
	ResultTable getEntityAndSecondaryAttribute(Entity&);


	ResultTable getEntityWithAttribute(Entity&, Entity&);
	ResultTable getEntityWithSecondaryAttribute(Entity&, Entity&);
};
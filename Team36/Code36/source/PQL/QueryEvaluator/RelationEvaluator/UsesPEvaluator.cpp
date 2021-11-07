#include "PQL/QueryObject/Query.h"
#include "PQL/QueryEvaluator/QueryResult.h"
#include "PKB/PKBAdapter.h"
#include "UsesPEvaluator.h"
#include "PKB/RelationPreprocessorManager.h"

bool UsesPEvaluator::evaluateWildAndWild() {
	throw std::invalid_argument("evaluateWildAndWild(): Wild is not allowed for first argument of Uses_P");
}

bool UsesPEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	proc_name p = e1.getValue();
	var_name v = e2.getValue();
	return pkb.isUsesP(p, v);
}

bool UsesPEvaluator::evaluateConstantAndWild(Entity e) {
	proc_name p = e.getValue();
	return pkb.isUsesP(p);
}

bool UsesPEvaluator::evaluateWildAndConstant(Entity e) {
	throw std::invalid_argument("evaluateWildAndConstant(): Wild is not allowed for first argument of Uses_P");
}

ResultTable UsesPEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getUsesPRelation());
}

ResultTable UsesPEvaluator::evaluateWildAndSynonym(Entity header) {
	throw std::invalid_argument("evaluateWildAndSynonym(): Wild is not allowed for first argument of Uses_P");
}

ResultTable UsesPEvaluator::evaluateSynonymAndWild(Entity header) {
	return ResultTable(header, pkb.getUsesP());
}

ResultTable UsesPEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	proc_name p = constant.getValue();
	return ResultTable(header, pkb.getUsedP(p));
}

ResultTable UsesPEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	var_name v = constant.getValue();
	return ResultTable(header, pkb.getUsesP(v));
}
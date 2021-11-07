#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "ModifiesPEvaluator.h"

bool ModifiesPEvaluator::evaluateWildAndWild() {
	throw std::invalid_argument("evaluateWildAndWild(): Wild is not allowed for first argument of Modifies_P");
}

bool ModifiesPEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	proc_name p = e1.getValue();
	var_name v = e2.getValue();
	return pkb.isModifiesP(p, v);
}

bool ModifiesPEvaluator::evaluateConstantAndWild(Entity e) {
	proc_name p = e.getValue();
	return pkb.isModifiesP(p);
}

bool ModifiesPEvaluator::evaluateWildAndConstant(Entity e) {
	throw std::invalid_argument("evaluateWildAndConstant(): Wild is not allowed for first argument of Modifies_P");
}

ResultTable ModifiesPEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getModifiesPRelation());
}

ResultTable ModifiesPEvaluator::evaluateWildAndSynonym(Entity header) {
	throw std::invalid_argument("evaluateWildAndSynonym(): Wild is not allowed for first argument of Modifies_P");
}

ResultTable ModifiesPEvaluator::evaluateSynonymAndWild(Entity header) {
	return ResultTable(header, pkb.getModifiesP());
}

ResultTable ModifiesPEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	proc_name p = constant.getValue();
	return ResultTable(header, pkb.getModifiedP(p));
}

ResultTable ModifiesPEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	var_name v = constant.getValue();
	return ResultTable(header, pkb.getModifiesP(v));
}
#include "PQL/QueryObject/Query.h"
#include "PQL/QueryEvaluator/QueryResult.h"
#include "PKB/PKBAdapter.h"
#include "UsesSEvaluator.h"
#include "PKB/RelationPreprocessorManager.h"

bool UsesSEvaluator::evaluateWildAndWild() {
	throw std::invalid_argument("evaluateWildAndWild(): Wild is not allowed for first argument of Uses_S");
}

bool UsesSEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	stmt_index s = stoi(e1.getValue());
	var_name v = e2.getValue();
	return pkb.isUsesS(s, v);
}

bool UsesSEvaluator::evaluateConstantAndWild(Entity e) {
	stmt_index s = stoi(e.getValue());
	return pkb.isUsesS(s);
}

bool UsesSEvaluator::evaluateWildAndConstant(Entity e) {
	throw std::invalid_argument("evaluateWildAndConstant(): Wild is not allowed for first argument of Uses_S");
}

ResultTable UsesSEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getUsesSRelation());
}

ResultTable UsesSEvaluator::evaluateWildAndSynonym(Entity header) {
	throw std::invalid_argument("evaluateWildAndSynonym(): Wild is not allowed for first argument of Uses_S");
}

ResultTable UsesSEvaluator::evaluateSynonymAndWild(Entity header) {
	return ResultTable(header, pkb.getUsesS());
}

ResultTable UsesSEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	stmt_index s = stoi(constant.getValue());
	return ResultTable(header, pkb.getUsedS(s));
}

ResultTable UsesSEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	var_name v = constant.getValue();
	return ResultTable(header, pkb.getUsesS(v));
}
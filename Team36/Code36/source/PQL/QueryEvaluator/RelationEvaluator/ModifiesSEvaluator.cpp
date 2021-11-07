#include "PQL/QueryObject/Query.h"
#include "PQL/QueryEvaluator/QueryResult.h"
#include "PKB/PKBAdapter.h"
#include "ModifiesSEvaluator.h"
#include "PKB/RelationPreprocessorManager.h"

bool ModifiesSEvaluator::evaluateWildAndWild() {
	throw std::invalid_argument("evaluateWildAndWild(): Wild is not allowed for first argument of Modifies_S");
}

bool ModifiesSEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	stmt_index s = stoi(e1.getValue());
	var_name v = e2.getValue();
	return pkb.isModifiesS(s, v);
}

bool ModifiesSEvaluator::evaluateConstantAndWild(Entity e) {
	stmt_index s = stoi(e.getValue());
	return pkb.isModifiesS(s);
}

bool ModifiesSEvaluator::evaluateWildAndConstant(Entity e) {
	throw std::invalid_argument("evaluateWildAndConstant(): Wild is not allowed for first argument of Modifies_S");
}

ResultTable ModifiesSEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getModifiesSRelation());
}

ResultTable ModifiesSEvaluator::evaluateWildAndSynonym(Entity header) {
	throw std::invalid_argument("evaluateWildAndSynonym(): Wild is not allowed for first argument of Modifies_S");
}

ResultTable ModifiesSEvaluator::evaluateSynonymAndWild(Entity header) {
	return ResultTable(header, pkb.getModifiesS());
}

ResultTable ModifiesSEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	stmt_index s = stoi(constant.getValue());
	return ResultTable(header, pkb.getModifiedS(s));
}

ResultTable ModifiesSEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	var_name v = constant.getValue();
	return ResultTable(header, pkb.getModifiesS(v));
}
#include "PQL/QueryObject/Query.h"
#include "PQL/QueryEvaluator/QueryResult.h"
#include "PKB/PKBAdapter.h"
#include "CallsEvaluator.h"
#include "PKB/RelationPreprocessorManager.h"

bool CallsEvaluator::evaluateWildAndWild() {
	return !pkb.isCallsPEmpty();;
}

bool CallsEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	proc_name caller = e1.getValue();
	proc_name callee = e2.getValue();
	return pkb.isCallsP(caller, callee);
}

bool CallsEvaluator::evaluateConstantAndWild(Entity e) {
	proc_name caller = e.getValue();
	return pkb.isCallerP(caller);
}

bool CallsEvaluator::evaluateWildAndConstant(Entity e) {
	proc_name callee = e.getValue();
	return pkb.isCalleeP(callee);
}

ResultTable CallsEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getCallsPRelation());
}

ResultTable CallsEvaluator::evaluateWildAndSynonym(Entity header) {
	return ResultTable(header, pkb.getCalleeP());
}

ResultTable CallsEvaluator::evaluateSynonymAndWild(Entity header) {
	return ResultTable(header, pkb.getCallerP());
}

ResultTable CallsEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	proc_name p = constant.getValue();
	return ResultTable(header, pkb.getCalleeP(p));
}

ResultTable CallsEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	proc_name p = constant.getValue();
	return ResultTable(header, pkb.getCallerP(p));
}
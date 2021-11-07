#include "PQL/QueryObject/Query.h"
#include "PQL/QueryEvaluator/QueryResult.h"
#include "PKB/PKBAdapter.h"
#include "CallsTEvaluator.h"
#include "PKB/RelationPreprocessorManager.h"

bool CallsTEvaluator::evaluateWildAndWild() {
	return !pkb.isCallsPTEmpty();
}

bool CallsTEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	proc_name caller = e1.getValue();
	proc_name callee = e2.getValue();
	return pkb.isCallsPT(caller, callee);
}

bool CallsTEvaluator::evaluateConstantAndWild(Entity e) {
	proc_name caller = e.getValue();
	return pkb.isCallerPT(caller);
}

bool CallsTEvaluator::evaluateWildAndConstant(Entity e) {
	proc_name caller = e.getValue();
	return pkb.isCalleePT(caller);
}

ResultTable CallsTEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getCallsPTRelation());
}

ResultTable CallsTEvaluator::evaluateWildAndSynonym(Entity header) {
	return ResultTable(header, pkb.getCalleePT());
}

ResultTable CallsTEvaluator::evaluateSynonymAndWild(Entity header) {
	return ResultTable(header, pkb.getCallerPT());
}

ResultTable CallsTEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	proc_name p = constant.getValue();
	return ResultTable(header, pkb.getCalleePT(p));
}

ResultTable CallsTEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	proc_name p = constant.getValue();
	return ResultTable(header, pkb.getCallerPT(p));
}
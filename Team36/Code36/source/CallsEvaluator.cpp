#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "CallsEvaluator.h"

bool CallsEvaluator::haveRelation() {
	return !pkb.isCallsPEmpty();;
}

bool CallsEvaluator::isRelation(Entity e1, Entity e2) {
	proc_name caller = e1.getValue();
	proc_name callee = e2.getValue();
	return pkb.isCallsP(caller, callee);
}

bool CallsEvaluator::haveRelationAtRight(Entity e) {
	proc_name caller = e.getValue();
	return pkb.isCallerP(caller);
}

bool CallsEvaluator::haveRelationAtLeft(Entity e) {
	proc_name callee = e.getValue();
	return pkb.isCalleeP(callee);
}

ResultTable CallsEvaluator::getRelations(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getCallsPRelation());
}

ResultTable CallsEvaluator::getRightRelations(Entity header) {
	return ResultTable(header, pkb.getCalleeP());
}

ResultTable CallsEvaluator::getLeftRelations(Entity header) {
	return ResultTable(header, pkb.getCallerP());
}

ResultTable CallsEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	proc_name p = constant.getValue();
	return ResultTable(header, pkb.getCalleeP(p));
}

ResultTable CallsEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	proc_name p = constant.getValue();
	return ResultTable(header, pkb.getCallerP(p));
}
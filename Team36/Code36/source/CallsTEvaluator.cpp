#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "CallsTEvaluator.h"

bool CallsTEvaluator::haveRelation() {
	return !pkb.isCallsPTEmpty();
}

bool CallsTEvaluator::isRelation(Entity e1, Entity e2) {
	proc_name caller = e1.getValue();
	proc_name callee = e2.getValue();
	return pkb.isCallsPT(caller, callee);
}

bool CallsTEvaluator::haveRelationAtRight(Entity e) {
	proc_name caller = e.getValue();
	return pkb.isCallerPT(caller);
}

bool CallsTEvaluator::haveRelationAtLeft(Entity e) {
	proc_name caller = e.getValue();
	return pkb.isCalleePT(caller);
}

ResultTable CallsTEvaluator::getRelations(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getCallsPTRelation());
}

ResultTable CallsTEvaluator::getRightRelations(Entity header) {
	return ResultTable(header, pkb.getCalleePT());
}

ResultTable CallsTEvaluator::getLeftRelations(Entity header) {
	return ResultTable(header, pkb.getCallerPT());
}

ResultTable CallsTEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	proc_name p = constant.getValue();
	return ResultTable(header, pkb.getCalleePT(p));
}

ResultTable CallsTEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	proc_name p = constant.getValue();
	return ResultTable(header, pkb.getCallerPT(p));
}
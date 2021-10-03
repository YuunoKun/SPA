#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "CallsTEvaluator.h"

//Handle both wild : e.g Relation(_, _)
bool CallsTEvaluator::haveRelation() {
	return !pkb.isCallsPTEmpty();
}

//Handle both constant : e.g Relation(1, 2)
bool CallsTEvaluator::isRelation(Entity e1, Entity e2) {
	proc_name caller = e1.getValue();
	proc_name callee = e2.getValue();
	return pkb.isCallsPT(caller, callee);
}

//Handle left constant, right wild: e.g Relation(1, _)
bool CallsTEvaluator::haveRelationAtRight(Entity e) {
	proc_name caller = e.getValue();
	return pkb.isCallerPT(caller);
}

//Handle right wild, left constant: e.g Relation(_, "x")
bool CallsTEvaluator::haveRelationAtLeft(Entity e) {
	proc_name caller = e.getValue();
	return pkb.isCalleePT(caller);
}

//If both side is declartion: e.g Relation(a, b)
ResultTable CallsTEvaluator::getRelations(Entity left, Entity right) {
	std::vector<std::pair<std::string, std::string>> results = pkb.getCallsPTRelation();
	std::pair<Entity, Entity> header{ left, right };
	ResultTable result = ResultTable(header, results);
	return result;
}

//If left side is WILD and right side is declartion: e.g Relation(_, a)
ResultTable CallsTEvaluator::getRightRelations(Entity header) {
	return ResultTable(header, pkb.getCalleePT());
}

//Handle right declartion, left constant: e.g Relation(a, _)
ResultTable CallsTEvaluator::getLeftRelations(Entity header) {
	return ResultTable(header, pkb.getCallerPT());
}

//Handle left constant, right declartion: e.g Relation(1, a)
ResultTable CallsTEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	proc_name p = constant.getValue();
	return ResultTable(header, pkb.getCalleePT(p));
}

//Handle right declartion, left constant: e.g Relation(a, 1)
ResultTable CallsTEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	proc_name p = constant.getValue();
	return ResultTable(header, pkb.getCallerPT(p));
}
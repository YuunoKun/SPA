#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "CallsEvaluator.h"

//Handle both wild : e.g Relation(_, _)
bool CallsEvaluator::haveRelation() {
	return !pkb.isCallsPEmpty();;
}

//Handle both constant : e.g Relation(1, 2)
bool CallsEvaluator::isRelation(Entity e1, Entity e2) {
	proc_name caller = e1.getValue();
	proc_name callee = e2.getValue();
	return pkb.isCallsP(caller, callee);
}

//Handle left constant, right wild: e.g Relation(1, _)
bool CallsEvaluator::haveRelationAtRight(Entity e) {
	proc_name caller = e.getValue();
	return pkb.isCallerP(caller);
}

//Handle right wild, left constant: e.g Relation(_, "x")
bool CallsEvaluator::haveRelationAtLeft(Entity e) {
	proc_name caller = e.getValue();
	return pkb.isCalleeP(caller);
}

//If both side is declartion: e.g Relation(a, b)
ResultTable CallsEvaluator::getRelations(Entity left, Entity right) {
	std::vector<std::pair<std::string, std::string>> results = pkb.getCallsPRelation();
	std::pair<Entity, Entity> header{ left, right };
	ResultTable result = ResultTable(header, results);
	return result;
}

//If left side is WILD and right side is declartion: e.g Relation(_, a)
ResultTable CallsEvaluator::getRightRelations(Entity header) {
	return ResultTable(header, pkb.getCalleeP());
}

//Handle right declartion, left constant: e.g Relation(a, _)
ResultTable CallsEvaluator::getLeftRelations(Entity header) {
	return ResultTable(header, pkb.getCallerP());
}

//Handle left constant, right declartion: e.g Relation(1, a)
ResultTable CallsEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	proc_name p = constant.getValue();
	return ResultTable(header, pkb.getCalleeP(p));
}

//Handle right declartion, left constant: e.g Relation(a, 1)
ResultTable CallsEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	proc_name p = constant.getValue();
	return ResultTable(header, pkb.getCallerP(p));
}
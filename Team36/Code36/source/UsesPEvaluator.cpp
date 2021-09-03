#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "UsesPEvaluator.h"

//Handle both wild : e.g Relation(_, _)
bool UsesPEvaluator::haveRelation() {
	throw std::invalid_argument("haveRelation(): Wild is not allowed for first argument of Uses_P");
}

//Handle both constant : e.g Relation(1, 2)
bool UsesPEvaluator::isRelation(Entity e1, Entity e2) {
	proc_name p = e1.getName();
	var_name v = e2.getName();
	return pkb.isUses(p, v);
}

//Handle left constant, right wild: e.g Relation(1, _)
bool UsesPEvaluator::haveRelationAtRight(Entity e) {
	proc_name p = e.getName();
	return pkb.isUses(p);
}

//Handle right wild, left constant: e.g Relation(_, 1)
bool UsesPEvaluator::haveRelationAtLeft(Entity e) {
	throw std::invalid_argument("haveRelationAtLeft(): Wild is not allowed for first argument of Uses_P");
}

//If both side is declartion: e.g Relation(a, b)
ResultTable UsesPEvaluator::getRelations(Entity left, Entity right) {
	std::vector<std::pair<std::string, std::string>> results = pkb.getUsesPRelation();
	std::vector<Entity> header{ left, right };
	//TODO
	//ResultTable result = ResultTable(header, results);
	//return result;
	throw std::invalid_argument("TODO");
}

//If left side is WILD and right side is declartion: e.g Relation(_, a)
ResultTable UsesPEvaluator::getRightRelations(Entity header) {
	throw std::invalid_argument("getRightRelations(): Wild is not allowed for first argument of Uses_P");
}

//Handle right declartion, left constant: e.g Relation(a, _)
ResultTable UsesPEvaluator::getLeftRelations(Entity header) {
	return ResultTable(header, pkb.getUsesP());
}

//Handle left constant, right declartion: e.g Relation(1, a)
ResultTable UsesPEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	proc_name p = constant.getName();
	return ResultTable(header, pkb.getUsedP(p));
}

//Handle right declartion, left constant: e.g Relation(a, 1)
ResultTable UsesPEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	var_name v = constant.getName();
	return ResultTable(header, pkb.getUsesP(v));
}

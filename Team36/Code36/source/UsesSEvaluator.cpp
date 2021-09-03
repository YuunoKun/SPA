#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "UsesSEvaluator.h"

//Handle both wild : e.g Relation(_, _)
bool UsesSEvaluator::haveRelation() {
	throw std::invalid_argument("haveRelation(): Wild is not allowed for first argument of Uses_S");
}

//Handle both constant : e.g Relation(1, 2)
bool UsesSEvaluator::isRelation(Entity e1, Entity e2) {
	stmt_index s = stoi(e1.getName());
	var_name v = e2.getName();
	return pkb.isUses(s, v);
}

//Handle left constant, right wild: e.g Relation(1, _)
bool UsesSEvaluator::haveRelationAtRight(Entity e) {
	stmt_index s = stoi(e.getValue());
	return pkb.isUses(s);
}

//Handle right wild, left constant: e.g Relation(_, 1)
bool UsesSEvaluator::haveRelationAtLeft(Entity e) {
	throw std::invalid_argument("haveRelationAtLeft(): Wild is not allowed for first argument of Uses_S");
}

//If both side is declartion: e.g Relation(a, b)
ResultTable UsesSEvaluator::getRelations(Entity left, Entity right) {
	std::vector<std::pair<Stmt, std::string>> results = pkb.getUsesSRelation();
	std::vector<Entity> header{ left, right };
	//TODO
	//ResultTable result = ResultTable(header, results);
	//return result;
	throw std::invalid_argument("TODO");
}

//If left side is WILD and right side is declartion: e.g Relation(_, a)
ResultTable UsesSEvaluator::getRightRelations(Entity header) {
	throw std::invalid_argument("getRightRelations(): Wild is not allowed for first argument of Uses_S");
}

//Handle right declartion, left constant: e.g Relation(a, _)
ResultTable UsesSEvaluator::getLeftRelations(Entity header) {
	return ResultTable(header, pkb.getUsesS());
}

//Handle left constant, right declartion: e.g Relation(1, a)
ResultTable UsesSEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	stmt_index s = stoi(constant.getValue());
	return ResultTable(header, pkb.getUsedS(s));
}

//Handle right declartion, left constant: e.g Relation(a, 1)
ResultTable UsesSEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	var_name v = constant.getName();
	return ResultTable(header, pkb.getUsesS(v));
}


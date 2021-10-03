#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "AffectEvaluator.h"

//Handle both wild : e.g Relation(_, _)
bool AffectEvaluator::haveRelation() {
	//Todo
	throw std::invalid_argument("haveRelation(): Wild is not allowed for first argument of Uses_S");
}

//Handle both constant : e.g Relation(1, 2)
bool AffectEvaluator::isRelation(Entity e1, Entity e2) {
	//Todo
	stmt_index s = stoi(e1.getValue());
	var_name v = e2.getValue();
	return pkb.isUsesS(s, v);
}

//Handle left constant, right wild: e.g Relation(1, _)
bool AffectEvaluator::haveRelationAtRight(Entity e) {
	//Todo
	stmt_index s = stoi(e.getValue());
	return pkb.isUsesS(s);
}

//Handle right wild, left constant: e.g Relation(_, "x")
bool AffectEvaluator::haveRelationAtLeft(Entity e) {
	//Todo
	throw std::invalid_argument("haveRelationAtLeft(): Wild is not allowed for first argument of Uses_S");
}

//If both side is declartion: e.g Relation(a, b)
ResultTable AffectEvaluator::getRelations(Entity left, Entity right) {
	//Todo
	std::vector<std::pair<StmtInfo, std::string>> results = pkb.getUsesSRelation();
	std::pair<Entity, Entity> header{ left, right };
	ResultTable result = ResultTable(header, results);
	return result;
}

//If left side is WILD and right side is declartion: e.g Relation(_, a)
ResultTable AffectEvaluator::getRightRelations(Entity header) {
	//Todo
	throw std::invalid_argument("getRightRelations(): Wild is not allowed for first argument of Uses_S");
}

//Handle right declartion, left constant: e.g Relation(a, _)
ResultTable AffectEvaluator::getLeftRelations(Entity header) {
	//Todo
	return ResultTable(header, pkb.getUsesS());
}

//Handle left constant, right declartion: e.g Relation(1, a)
ResultTable AffectEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	//Todo
	stmt_index s = stoi(constant.getValue());
	return ResultTable(header, pkb.getUsedS(s));
}

//Handle right declartion, left constant: e.g Relation(a, 1)
ResultTable AffectEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	//Todo
	var_name v = constant.getValue();
	return ResultTable(header, pkb.getUsesS(v));
}
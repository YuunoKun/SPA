#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "ModifiesSEvaluator.h"


//Handle both wild : e.g Relation(_, _)
bool ModifiesSEvaluator::haveRelation() {
	return !pkb.getModifiesS().empty();
	//Todo: throw exception for iteration 2
	//throw std::invalid_argument("haveRelation(): Wild is not allowed for first argument of Modifies_S");
}

//Handle both constant : e.g Relation(1, 2)
bool ModifiesSEvaluator::isRelation(Entity e1, Entity e2) {
	stmt_index s = stoi(e1.getName());
	var_name v = e2.getName();
	return pkb.isModifiesS(s, v);
}

//Handle left constant, right wild: e.g Relation(1, _)
bool ModifiesSEvaluator::haveRelationAtRight(Entity e) {
	stmt_index s = stoi(e.getValue());
	return pkb.isModifiesS(s);
}

//Handle right wild, left constant: e.g Relation(_, 1)
bool ModifiesSEvaluator::haveRelationAtLeft(Entity e) {
	var_name v = e.getName();
	return pkb.isModifiesS(v);
	//Todo: throw exception for iteration 2
	//throw std::invalid_argument("haveRelationAtLeft(): Wild is not allowed for first argument of Modifies_S");
}

//If both side is declartion: e.g Relation(a, b)
ResultTable ModifiesSEvaluator::getRelations(Entity left, Entity right) {
	std::vector<std::pair<StmtInfo, std::string>> results = pkb.getModifiesSRelation();
	std::vector<Entity> header{ left, right };
	//TODO
	//ResultTable result = ResultTable(header, results);
	//return result;
	throw std::invalid_argument("TODO");
}

//If left side is WILD and right side is declartion: e.g Relation(_, a)
ResultTable ModifiesSEvaluator::getRightRelations(Entity header) {
	return ResultTable(header, pkb.getModifiedS());
	//Todo: throw exception for iteration 2
	//throw std::invalid_argument("getRightRelations(): Wild is not allowed for first argument of Modifies_S");
}

//Handle right declartion, left constant: e.g Relation(a, _)
ResultTable ModifiesSEvaluator::getLeftRelations(Entity header) {
	return ResultTable(header, pkb.getModifiesS());
}

//Handle left constant, right declartion: e.g Relation(1, a)
ResultTable ModifiesSEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	stmt_index s = stoi(constant.getValue());
	return ResultTable(header, pkb.getModifiedS(s));
}

//Handle right declartion, left constant: e.g Relation(a, 1)
ResultTable ModifiesSEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	var_name v = constant.getName();
	return ResultTable(header, pkb.getModifiesS(v));
}

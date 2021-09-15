#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "ModifiesPEvaluator.h"

//Handle both wild : e.g Relation(_, _)
bool ModifiesPEvaluator::haveRelation() {
	throw std::invalid_argument("haveRelation(): Wild is not allowed for first argument of Modifies_P");
}

//Handle both constant : e.g Relation(1, 2)
bool ModifiesPEvaluator::isRelation(Entity e1, Entity e2) {
	proc_name p = e1.getValue();
	var_name v = e2.getValue();
	return pkb.isModifiesP(p, v);
}

//Handle left constant, right wild: e.g Relation(1, _)
bool ModifiesPEvaluator::haveRelationAtRight(Entity e) {
	proc_name p = e.getValue();
	return pkb.isModifiesP(p);
}

//Handle right wild, left constant: e.g Relation(_, 1)
bool ModifiesPEvaluator::haveRelationAtLeft(Entity e) {
	throw std::invalid_argument("haveRelationAtLeft(): Wild is not allowed for first argument of Modifies_P");
}

//If both side is declartion: e.g Relation(a, b)
ResultTable ModifiesPEvaluator::getRelations(Entity left, Entity right) {
	std::vector<std::pair<std::string, std::string>> results = pkb.getModifiesPRelation();
	std::pair<Entity, Entity> header{ left, right };
	ResultTable result = ResultTable(header, results);
	return result;
}

//If left side is WILD and right side is declartion: e.g Relation(_, a)
ResultTable ModifiesPEvaluator::getRightRelations(Entity header) {
	throw std::invalid_argument("getRightRelations(): Wild is not allowed for first argument of Modifies_P");
}

//Handle right declartion, left constant: e.g Relation(a, _)
ResultTable ModifiesPEvaluator::getLeftRelations(Entity header) {
	return ResultTable(header, pkb.getModifiesP());
}

//Handle left constant, right declartion: e.g Relation(1, a)
ResultTable ModifiesPEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	proc_name p = constant.getValue();
	return ResultTable(header, pkb.getModifiedP(p));
}

//Handle right declartion, left constant: e.g Relation(a, 1)
ResultTable ModifiesPEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	var_name v = constant.getValue();
	return ResultTable(header, pkb.getModifiesP(v));
}
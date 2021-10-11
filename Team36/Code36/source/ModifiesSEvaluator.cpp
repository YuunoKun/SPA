#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "ModifiesSEvaluator.h"

bool ModifiesSEvaluator::haveRelation() {
	throw std::invalid_argument("haveRelation(): Wild is not allowed for first argument of Modifies_S");
}

bool ModifiesSEvaluator::isRelation(Entity e1, Entity e2) {
	stmt_index s = stoi(e1.getValue());
	var_name v = e2.getValue();
	return pkb.isModifiesS(s, v);
}

bool ModifiesSEvaluator::haveRelationAtRight(Entity e) {
	stmt_index s = stoi(e.getValue());
	return pkb.isModifiesS(s);
}

bool ModifiesSEvaluator::haveRelationAtLeft(Entity e) {
	throw std::invalid_argument("haveRelationAtLeft(): Wild is not allowed for first argument of Modifies_S");
}

ResultTable ModifiesSEvaluator::getRelations(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getModifiesSRelation());
}

ResultTable ModifiesSEvaluator::getRightRelations(Entity header) {
	throw std::invalid_argument("getRightRelations(): Wild is not allowed for first argument of Modifies_S");
}

ResultTable ModifiesSEvaluator::getLeftRelations(Entity header) {
	return ResultTable(header, pkb.getModifiesS());
}

ResultTable ModifiesSEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	stmt_index s = stoi(constant.getValue());
	return ResultTable(header, pkb.getModifiedS(s));
}

ResultTable ModifiesSEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	var_name v = constant.getValue();
	return ResultTable(header, pkb.getModifiesS(v));
}
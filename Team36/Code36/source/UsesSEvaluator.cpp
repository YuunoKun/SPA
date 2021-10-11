#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "UsesSEvaluator.h"

bool UsesSEvaluator::haveRelation() {
	throw std::invalid_argument("haveRelation(): Wild is not allowed for first argument of Uses_S");
}

bool UsesSEvaluator::isRelation(Entity e1, Entity e2) {
	stmt_index s = stoi(e1.getValue());
	var_name v = e2.getValue();
	return pkb.isUsesS(s, v);
}

bool UsesSEvaluator::haveRelationAtRight(Entity e) {
	stmt_index s = stoi(e.getValue());
	return pkb.isUsesS(s);
}

bool UsesSEvaluator::haveRelationAtLeft(Entity e) {
	throw std::invalid_argument("haveRelationAtLeft(): Wild is not allowed for first argument of Uses_S");
}

ResultTable UsesSEvaluator::getRelations(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getUsesSRelation());
}

ResultTable UsesSEvaluator::getRightRelations(Entity header) {
	throw std::invalid_argument("getRightRelations(): Wild is not allowed for first argument of Uses_S");
}

ResultTable UsesSEvaluator::getLeftRelations(Entity header) {
	return ResultTable(header, pkb.getUsesS());
}

ResultTable UsesSEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	stmt_index s = stoi(constant.getValue());
	return ResultTable(header, pkb.getUsedS(s));
}

ResultTable UsesSEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	var_name v = constant.getValue();
	return ResultTable(header, pkb.getUsesS(v));
}
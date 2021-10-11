#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "ModifiesPEvaluator.h"

bool ModifiesPEvaluator::haveRelation() {
	throw std::invalid_argument("haveRelation(): Wild is not allowed for first argument of Modifies_P");
}

bool ModifiesPEvaluator::isRelation(Entity e1, Entity e2) {
	proc_name p = e1.getValue();
	var_name v = e2.getValue();
	return pkb.isModifiesP(p, v);
}

bool ModifiesPEvaluator::haveRelationAtRight(Entity e) {
	proc_name p = e.getValue();
	return pkb.isModifiesP(p);
}

bool ModifiesPEvaluator::haveRelationAtLeft(Entity e) {
	throw std::invalid_argument("haveRelationAtLeft(): Wild is not allowed for first argument of Modifies_P");
}

ResultTable ModifiesPEvaluator::getRelations(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getModifiesPRelation());
}

ResultTable ModifiesPEvaluator::getRightRelations(Entity header) {
	throw std::invalid_argument("getRightRelations(): Wild is not allowed for first argument of Modifies_P");
}

ResultTable ModifiesPEvaluator::getLeftRelations(Entity header) {
	return ResultTable(header, pkb.getModifiesP());
}

ResultTable ModifiesPEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	proc_name p = constant.getValue();
	return ResultTable(header, pkb.getModifiedP(p));
}

ResultTable ModifiesPEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	var_name v = constant.getValue();
	return ResultTable(header, pkb.getModifiesP(v));
}
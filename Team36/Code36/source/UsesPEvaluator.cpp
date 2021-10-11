#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "UsesPEvaluator.h"

bool UsesPEvaluator::haveRelation() {
	throw std::invalid_argument("haveRelation(): Wild is not allowed for first argument of Uses_P");
}

bool UsesPEvaluator::isRelation(Entity e1, Entity e2) {
	proc_name p = e1.getValue();
	var_name v = e2.getValue();
	return pkb.isUsesP(p, v);
}

bool UsesPEvaluator::haveRelationAtRight(Entity e) {
	proc_name p = e.getValue();
	return pkb.isUsesP(p);
}

bool UsesPEvaluator::haveRelationAtLeft(Entity e) {
	throw std::invalid_argument("haveRelationAtLeft(): Wild is not allowed for first argument of Uses_P");
}

ResultTable UsesPEvaluator::getRelations(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getUsesPRelation());
}

ResultTable UsesPEvaluator::getRightRelations(Entity header) {
	throw std::invalid_argument("getRightRelations(): Wild is not allowed for first argument of Uses_P");
}

ResultTable UsesPEvaluator::getLeftRelations(Entity header) {
	return ResultTable(header, pkb.getUsesP());
}

ResultTable UsesPEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	proc_name p = constant.getValue();
	return ResultTable(header, pkb.getUsedP(p));
}

ResultTable UsesPEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	var_name v = constant.getValue();
	return ResultTable(header, pkb.getUsesP(v));
}
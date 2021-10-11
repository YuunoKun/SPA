#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "ParentTEvaluator.h"

bool ParentTEvaluator::haveRelation() {
	return !pkb.isParentTEmpty();
}

bool ParentTEvaluator::isRelation(Entity e1, Entity e2) {
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.isParentT(c1, c2);
}

bool ParentTEvaluator::haveRelationAtRight(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isParentT(c);
}

bool ParentTEvaluator::haveRelationAtLeft(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isChildT(c);
}

ResultTable ParentTEvaluator::getRelations(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getAllParentTRelation());
}

ResultTable ParentTEvaluator::getRightRelations(Entity header) {
	return ResultTable(header, pkb.getChildT());
}

ResultTable ParentTEvaluator::getLeftRelations(Entity header) {
	return ResultTable(header, pkb.getParentT());
}

ResultTable ParentTEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getChildT(c));
}

ResultTable ParentTEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getParentT(c));
}
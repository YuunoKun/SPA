#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "ParentEvaluator.h"

bool ParentEvaluator::haveRelation() {
	return !pkb.isParentEmpty();
}

bool ParentEvaluator::isRelation(Entity e1, Entity e2) {
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.isParent(c1, c2);
}

bool ParentEvaluator::haveRelationAtRight(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isParent(c);
}

bool ParentEvaluator::haveRelationAtLeft(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isChild(c);
}

ResultTable ParentEvaluator::getRelations(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getAllParentRelation());
}

ResultTable ParentEvaluator::getRightRelations(Entity header) {
	return ResultTable(header, pkb.getChild());
}

ResultTable ParentEvaluator::getLeftRelations(Entity header) {
	return ResultTable(header, pkb.getParent());
}

ResultTable ParentEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getChild(c));
}

ResultTable ParentEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getParent(c));
}
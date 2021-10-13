#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "ParentTEvaluator.h"

bool ParentTEvaluator::evaluateWildAndWild() {
	return !pkb.isParentTEmpty();
}

bool ParentTEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.isParentT(c1, c2);
}

bool ParentTEvaluator::evaluateConstantAndWild(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isParentT(c);
}

bool ParentTEvaluator::evaluateWildAndConstant(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isChildT(c);
}

ResultTable ParentTEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getAllParentTRelation());
}

ResultTable ParentTEvaluator::evaluateWildAndSynonym(Entity header) {
	return ResultTable(header, pkb.getChildT());
}

ResultTable ParentTEvaluator::evaluateSynonymAndWild(Entity header) {
	return ResultTable(header, pkb.getParentT());
}

ResultTable ParentTEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getChildT(c));
}

ResultTable ParentTEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getParentT(c));
}
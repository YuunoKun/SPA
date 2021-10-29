#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "NextEvaluator.h"

bool NextEvaluator::evaluateWildAndWild() {
	return !pkb.isNextEmpty();
}

bool NextEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.isNext(c1, c2);;
}

bool NextEvaluator::evaluateConstantAndWild(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isPrevious(c);
}

bool NextEvaluator::evaluateWildAndConstant(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isNext(c);
}

ResultTable NextEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getAllNextRelation());
}

ResultTable NextEvaluator::evaluateWildAndSynonym(Entity header) {
	return ResultTable(header, pkb.getNext());
}

ResultTable NextEvaluator::evaluateSynonymAndWild(Entity header) {
	return ResultTable(header, pkb.getPrevious());
}

ResultTable NextEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getNext(c));
}

ResultTable NextEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getPrevious(c));
}
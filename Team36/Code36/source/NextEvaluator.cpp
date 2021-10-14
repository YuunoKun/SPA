#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "NextEvaluator.h"

bool NextEvaluator::evaluateWildAndWild() {
	//Todo Evaluator
	return !pkb.isNextEmpty();
}

bool NextEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	//Todo Evaluator
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.isNext(c1, c2);;
}

bool NextEvaluator::evaluateConstantAndWild(Entity e) {
	//Todo Evaluator
	stmt_index c = stoi(e.getValue());
	return pkb.isPrevious(c);
}

bool NextEvaluator::evaluateWildAndConstant(Entity e) {
	//Todo Evaluator
	stmt_index c = stoi(e.getValue());
	return pkb.isNext(c);
}

ResultTable NextEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getAllNextRelation());
}

ResultTable NextEvaluator::evaluateWildAndSynonym(Entity header) {
	//Todo Evaluator
	return ResultTable(header, pkb.getNext());
}

ResultTable NextEvaluator::evaluateSynonymAndWild(Entity header) {
	//Todo Evaluator
	return ResultTable(header, pkb.getPrevious());
}

ResultTable NextEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	//Todo Evaluator
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getNext(c));
}

ResultTable NextEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	//Todo Evaluator
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getPrevious(c));
}
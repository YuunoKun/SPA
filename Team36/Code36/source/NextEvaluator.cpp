#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "NextEvaluator.h"

bool NextEvaluator::haveRelation() {
	//Todo Evaluator
	return !pkb.isNextEmpty();
}

bool NextEvaluator::isRelation(Entity e1, Entity e2) {
	//Todo Evaluator
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.isNext(c1, c2);;
}

bool NextEvaluator::haveRelationAtRight(Entity e) {
	//Todo Evaluator
	stmt_index c = stoi(e.getValue());
	return pkb.isPrevious(c);
}

bool NextEvaluator::haveRelationAtLeft(Entity e) {
	//Todo Evaluator
	stmt_index c = stoi(e.getValue());
	return pkb.isNext(c);
}

ResultTable NextEvaluator::getRelations(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getAllNextRelation());
}

ResultTable NextEvaluator::getRightRelations(Entity header) {
	//Todo Evaluator
	return ResultTable(header, pkb.getNext());
}

ResultTable NextEvaluator::getLeftRelations(Entity header) {
	//Todo Evaluator
	return ResultTable(header, pkb.getPrevious());
}

ResultTable NextEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	//Todo Evaluator
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getNext(c));
}

ResultTable NextEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	//Todo Evaluator
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getPrevious(c));
}
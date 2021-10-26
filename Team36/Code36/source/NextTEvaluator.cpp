#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "NextTEvaluator.h"
#include "CFGRelationsManager.h"

bool NextTEvaluator::evaluateWildAndWild() {
	return !CFGRelationsManager::getInstance().isNextTEmpty();
}

bool NextTEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	prog_line c1 = stoi(e1.getValue());
	prog_line c2 = stoi(e2.getValue());
	return CFGRelationsManager::getInstance().isNextT(c1, c2);;
}

bool NextTEvaluator::evaluateConstantAndWild(Entity e) {
	prog_line c = stoi(e.getValue());
	return CFGRelationsManager::getInstance().isPreviousT(c);
}

bool NextTEvaluator::evaluateWildAndConstant(Entity e) {
	prog_line c = stoi(e.getValue());
	return CFGRelationsManager::getInstance().isNextT(c);
}

ResultTable NextTEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, CFGRelationsManager::getInstance().getAllNextTRelation());
}

ResultTable NextTEvaluator::evaluateWildAndSynonym(Entity header) {
	return ResultTable(header, CFGRelationsManager::getInstance().getNextT());
}

ResultTable NextTEvaluator::evaluateSynonymAndWild(Entity header) {
	return ResultTable(header, CFGRelationsManager::getInstance().getPreviousT());
}

ResultTable NextTEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	prog_line c = stoi(constant.getValue());
	return ResultTable(header, CFGRelationsManager::getInstance().getNextT(c));
}

ResultTable NextTEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	prog_line c = stoi(constant.getValue());
	return ResultTable(header, CFGRelationsManager::getInstance().getPreviousT(c));
}
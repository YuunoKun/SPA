#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "AffectEvaluator.h"
#include "CFGRelationsManager.h"


bool AffectEvaluator::evaluateWildAndWild() {
	return !CFGRelationsManager::getInstance().isAffectsEmpty();
}

bool AffectEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return CFGRelationsManager::getInstance().isAffects(c1, c2);;
}

bool AffectEvaluator::evaluateConstantAndWild(Entity e) {
	stmt_index c = stoi(e.getValue());
	return CFGRelationsManager::getInstance().isAffector(c);
}

bool AffectEvaluator::evaluateWildAndConstant(Entity e) {
	stmt_index c = stoi(e.getValue());
	return CFGRelationsManager::getInstance().isAffected(c);
}

ResultTable AffectEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, CFGRelationsManager::getInstance().getAllAffectsRelation());
}

ResultTable AffectEvaluator::evaluateWildAndSynonym(Entity header) {
	return ResultTable(header, CFGRelationsManager::getInstance().getAffected());
}

ResultTable AffectEvaluator::evaluateSynonymAndWild(Entity header) {
	return ResultTable(header, CFGRelationsManager::getInstance().getAffector());
}

ResultTable AffectEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, CFGRelationsManager::getInstance().getAffected(c));
}

ResultTable AffectEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, CFGRelationsManager::getInstance().getAffector(c));
}
#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "AffectTEvaluator.h"
#include "CFGRelationsManager.h"

bool AffectTEvaluator::evaluateWildAndWild() {
	return !pkb.getRelationManager().isAffectsTEmpty();
}

bool AffectTEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.getRelationManager().isAffectsT(c1, c2);;
}

bool AffectTEvaluator::evaluateConstantAndWild(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.getRelationManager().isAffectorT(c);
}

bool AffectTEvaluator::evaluateWildAndConstant(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.getRelationManager().isAffectedT(c);
}

ResultTable AffectTEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getRelationManager().getAllAffectsTRelation());
}

ResultTable AffectTEvaluator::evaluateWildAndSynonym(Entity header) {
	return ResultTable(header, pkb.getRelationManager().getAffectedT());
}

ResultTable AffectTEvaluator::evaluateSynonymAndWild(Entity header) {
	return ResultTable(header, pkb.getRelationManager().getAffectorT());
}

ResultTable AffectTEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getRelationManager().getAffectedT(c));
}

ResultTable AffectTEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getRelationManager().getAffectorT(c));
}
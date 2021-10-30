#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "AffectsBipTEvaluator.h"
#include "CFGRelationsManager.h"

bool AffectsBipTEvaluator::evaluateWildAndWild() {
	return !pkb.getRelationManager().isAffectsBipTEmpty();
}

bool AffectsBipTEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.getRelationManager().isAffectsBipT(c1, c2);;
}

bool AffectsBipTEvaluator::evaluateConstantAndWild(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.getRelationManager().isAffectingBipT(c);
}

bool AffectsBipTEvaluator::evaluateWildAndConstant(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.getRelationManager().isAffectedBipT(c);
}

ResultTable AffectsBipTEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getRelationManager().getAllAffectsBipTRelation());
}

ResultTable AffectsBipTEvaluator::evaluateWildAndSynonym(Entity header) {
	return ResultTable(header, pkb.getRelationManager().getAffectedBipT());
}

ResultTable AffectsBipTEvaluator::evaluateSynonymAndWild(Entity header) {
	return ResultTable(header, pkb.getRelationManager().getAffectingBipT());
}

ResultTable AffectsBipTEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getRelationManager().getAffectedBipT(c));
}

ResultTable AffectsBipTEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getRelationManager().getAffectingBipT(c));
}
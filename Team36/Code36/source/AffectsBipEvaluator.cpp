#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "AffectsBipEvaluator.h"
#include "RelationPreprocessorManager.h"

bool AffectsBipEvaluator::evaluateWildAndWild() {
	return !pkb.getRelationManager().isAffectsBipEmpty();
}

bool AffectsBipEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.getRelationManager().isAffectsBip(c1, c2);;
}

bool AffectsBipEvaluator::evaluateConstantAndWild(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.getRelationManager().isAffectingBip(c);
}

bool AffectsBipEvaluator::evaluateWildAndConstant(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.getRelationManager().isAffectedBip(c);
}

ResultTable AffectsBipEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getRelationManager().getAllAffectsBipRelation());
}

ResultTable AffectsBipEvaluator::evaluateWildAndSynonym(Entity header) {
	return ResultTable(header, pkb.getRelationManager().getAffectedBip());
}

ResultTable AffectsBipEvaluator::evaluateSynonymAndWild(Entity header) {
	return ResultTable(header, pkb.getRelationManager().getAffectingBip());
}

ResultTable AffectsBipEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getRelationManager().getAffectedBip(c));
}

ResultTable AffectsBipEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getRelationManager().getAffectingBip(c));
}
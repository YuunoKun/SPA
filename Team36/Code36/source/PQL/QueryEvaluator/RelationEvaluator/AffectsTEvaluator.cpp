#include "PQL/QueryObject/Query.h"
#include "PQL/QueryEvaluator/QueryResult.h"
#include "PKB/PKBAdapter.h"
#include "AffectsTEvaluator.h"
#include "PKB/RelationPreprocessorManager.h"

bool AffectsTEvaluator::evaluateWildAndWild() {
	return !pkb.getRelationManager().isAffectsTEmpty();
}

bool AffectsTEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.getRelationManager().isAffectsT(c1, c2);;
}

bool AffectsTEvaluator::evaluateConstantAndWild(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.getRelationManager().isAffectingT(c);
}

bool AffectsTEvaluator::evaluateWildAndConstant(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.getRelationManager().isAffectedT(c);
}

ResultTable AffectsTEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getRelationManager().getAllAffectsTRelation());
}

ResultTable AffectsTEvaluator::evaluateWildAndSynonym(Entity header) {
	return ResultTable(header, pkb.getRelationManager().getAffectedT());
}

ResultTable AffectsTEvaluator::evaluateSynonymAndWild(Entity header) {
	return ResultTable(header, pkb.getRelationManager().getAffectingT());
}

ResultTable AffectsTEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getRelationManager().getAffectedT(c));
}

ResultTable AffectsTEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getRelationManager().getAffectingT(c));
}
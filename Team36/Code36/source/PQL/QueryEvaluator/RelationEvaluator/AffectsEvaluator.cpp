#include "PQL/QueryObject/Query.h"
#include "PQL/QueryEvaluator/QueryResult.h"
#include "PKB/PKBAdapter.h"
#include "AffectsEvaluator.h"
#include "PKB/RelationPreprocessorManager.h"

bool AffectsEvaluator::evaluateWildAndWild() {
	return !pkb.getRelationManager().isAffectsEmpty();
}

bool AffectsEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.getRelationManager().isAffects(c1, c2);;
}

bool AffectsEvaluator::evaluateConstantAndWild(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.getRelationManager().isAffecting(c);
}

bool AffectsEvaluator::evaluateWildAndConstant(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.getRelationManager().isAffected(c);
}

ResultTable AffectsEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getRelationManager().getAllAffectsRelation());
}

ResultTable AffectsEvaluator::evaluateWildAndSynonym(Entity header) {
	return ResultTable(header, pkb.getRelationManager().getAffected());
}

ResultTable AffectsEvaluator::evaluateSynonymAndWild(Entity header) {
	return ResultTable(header, pkb.getRelationManager().getAffecting());
}

ResultTable AffectsEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getRelationManager().getAffected(c));
}

ResultTable AffectsEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getRelationManager().getAffecting(c));
}
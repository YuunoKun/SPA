#include "PQL/QueryObject/Query.h"
#include "PQL/QueryEvaluator/QueryResult.h"
#include "PKB/PKBAdapter.h"
#include "NextBipEvaluator.h"
#include "PKB/RelationPreprocessorManager.h"

bool NextBipEvaluator::evaluateWildAndWild() {
	return !pkb.getRelationManager().isNextBipEmpty();
}

bool NextBipEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	prog_line c1 = stoi(e1.getValue());
	prog_line c2 = stoi(e2.getValue());
	return pkb.getRelationManager().isNextBip(c1, c2);;
}

bool NextBipEvaluator::evaluateConstantAndWild(Entity e) {
	prog_line c = stoi(e.getValue());
	return pkb.getRelationManager().isPreviousBip(c);
}

bool NextBipEvaluator::evaluateWildAndConstant(Entity e) {
	prog_line c = stoi(e.getValue());
	return pkb.getRelationManager().isNextBip(c);
}

ResultTable NextBipEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getRelationManager().getAllNextBipRelation());
}

ResultTable NextBipEvaluator::evaluateWildAndSynonym(Entity header) {
	return ResultTable(header, pkb.getRelationManager().getNextBip());
}

ResultTable NextBipEvaluator::evaluateSynonymAndWild(Entity header) {
	return ResultTable(header, pkb.getRelationManager().getPreviousBip());
}

ResultTable NextBipEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	prog_line c = stoi(constant.getValue());
	return ResultTable(header, pkb.getRelationManager().getNextBip(c));
}

ResultTable NextBipEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	prog_line c = stoi(constant.getValue());
	return ResultTable(header, pkb.getRelationManager().getPreviousBip(c));
}
#include "PQL/QueryObject/Query.h"
#include "PQL/QueryEvaluator/QueryResult.h"
#include "PKB/PKBAdapter.h"
#include "NextBipTEvaluator.h"
#include "PKB/RelationPreprocessorManager.h"

bool NextBipTEvaluator::evaluateWildAndWild() {
	return !pkb.getRelationManager().isNextBipTEmpty();
}

bool NextBipTEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	prog_line c1 = stoi(e1.getValue());
	prog_line c2 = stoi(e2.getValue());
	return pkb.getRelationManager().isNextBipT(c1, c2);;
}

bool NextBipTEvaluator::evaluateConstantAndWild(Entity e) {
	prog_line c = stoi(e.getValue());
	return pkb.getRelationManager().isPreviousBipT(c);
}

bool NextBipTEvaluator::evaluateWildAndConstant(Entity e) {
	prog_line c = stoi(e.getValue());
	return pkb.getRelationManager().isNextBipT(c);
}

ResultTable NextBipTEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getRelationManager().getAllNextBipTRelation());
}

ResultTable NextBipTEvaluator::evaluateWildAndSynonym(Entity header) {
	return ResultTable(header, pkb.getRelationManager().getNextBipT());
}

ResultTable NextBipTEvaluator::evaluateSynonymAndWild(Entity header) {
	return ResultTable(header, pkb.getRelationManager().getPreviousBipT());
}

ResultTable NextBipTEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	prog_line c = stoi(constant.getValue());
	return ResultTable(header, pkb.getRelationManager().getNextBipT(c));
}

ResultTable NextBipTEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	prog_line c = stoi(constant.getValue());
	return ResultTable(header, pkb.getRelationManager().getPreviousBipT(c));
}
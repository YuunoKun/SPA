#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "NextTEvaluator.h"
#include "RelationPreprocessorManager.h"

bool NextTEvaluator::evaluateWildAndWild() {
	return !pkb.getRelationManager().isNextTEmpty();
}

bool NextTEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	prog_line c1 = stoi(e1.getValue());
	prog_line c2 = stoi(e2.getValue());
	return pkb.getRelationManager().isNextT(c1, c2);;
}

bool NextTEvaluator::evaluateConstantAndWild(Entity e) {
	prog_line c = stoi(e.getValue());
	return pkb.getRelationManager().isPreviousT(c);
}

bool NextTEvaluator::evaluateWildAndConstant(Entity e) {
	prog_line c = stoi(e.getValue());
	return pkb.getRelationManager().isNextT(c);
}

ResultTable NextTEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getRelationManager().getAllNextTRelation());
}

ResultTable NextTEvaluator::evaluateWildAndSynonym(Entity header) {
	return ResultTable(header, pkb.getRelationManager().getNextT());
}

ResultTable NextTEvaluator::evaluateSynonymAndWild(Entity header) {
	return ResultTable(header, pkb.getRelationManager().getPreviousT());
}

ResultTable NextTEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	prog_line c = stoi(constant.getValue());
	return ResultTable(header, pkb.getRelationManager().getNextT(c));
}

ResultTable NextTEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	prog_line c = stoi(constant.getValue());
	return ResultTable(header, pkb.getRelationManager().getPreviousT(c));
}
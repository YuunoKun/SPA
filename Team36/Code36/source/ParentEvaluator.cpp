#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "ParentEvaluator.h"

bool ParentEvaluator::evaluateWildAndWild() {
	return !pkb.isParentEmpty();
}

bool ParentEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.isParent(c1, c2);
}

bool ParentEvaluator::evaluateConstantAndWild(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isParent(c);
}

bool ParentEvaluator::evaluateWildAndConstant(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isChild(c);
}

ResultTable ParentEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getAllParentRelation());
}

ResultTable ParentEvaluator::evaluateWildAndSynonym(Entity header) {
	return ResultTable(header, pkb.getChild());
}

ResultTable ParentEvaluator::evaluateSynonymAndWild(Entity header) {
	return ResultTable(header, pkb.getParent());
}

ResultTable ParentEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getChild(c));
}

ResultTable ParentEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getParent(c));
}
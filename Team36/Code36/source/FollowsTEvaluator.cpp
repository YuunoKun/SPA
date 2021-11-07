#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "FollowsTEvaluator.h"

bool FollowsTEvaluator::evaluateWildAndWild() {
	return !pkb.isFollowTEmpty();
}

bool FollowsTEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.isFollowT(c1, c2);
}

bool FollowsTEvaluator::evaluateConstantAndWild(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowedT(c);
}

bool FollowsTEvaluator::evaluateWildAndConstant(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowingT(c);
}

ResultTable FollowsTEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getFollowsT());
}

ResultTable FollowsTEvaluator::evaluateWildAndSynonym(Entity header) {
	return ResultTable(header, pkb.getFollowingT());
}

ResultTable FollowsTEvaluator::evaluateSynonymAndWild(Entity header) {
	return ResultTable(header, pkb.getFollowedT());
}

ResultTable FollowsTEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowingT(c));
}

ResultTable FollowsTEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowedT(c));
}
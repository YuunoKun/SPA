#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "FollowsEvaluator.h"

bool FollowsEvaluator::evaluateWildAndWild() {
	return !pkb.isFollowEmpty();
}

bool FollowsEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.isFollow(c1, c2);;
}

bool FollowsEvaluator::evaluateConstantAndWild(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowed(c);
}

bool FollowsEvaluator::evaluateWildAndConstant(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowing(c);
}

ResultTable FollowsEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getFollows());
}

ResultTable FollowsEvaluator::evaluateWildAndSynonym(Entity header) {
	return ResultTable(header, pkb.getFollowing());
}

ResultTable FollowsEvaluator::evaluateSynonymAndWild(Entity header) {
	return ResultTable(header, pkb.getFollowed());
}

ResultTable FollowsEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowing(c));
}

ResultTable FollowsEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowed(c));
}
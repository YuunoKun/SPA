#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "NextBipTEvaluator.h"


bool NextBipTEvaluator::evaluateWildAndWild() {
	//Todo Evaluator
	return !pkb.isFollowEmpty();
}

bool NextBipTEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	//Todo Evaluator
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.isFollow(c1, c2);;
}

bool NextBipTEvaluator::evaluateConstantAndWild(Entity e) {
	//Todo Evaluator
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowed(c);
}

bool NextBipTEvaluator::evaluateWildAndConstant(Entity e) {
	//Todo Evaluator
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowing(c);
}

ResultTable NextBipTEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	//Todo Evaluator
	return ResultTable({ left, right }, pkb.getFollows());
}

ResultTable NextBipTEvaluator::evaluateWildAndSynonym(Entity header) {
	//Todo Evaluator
	return ResultTable(header, pkb.getFollowing());
}

ResultTable NextBipTEvaluator::evaluateSynonymAndWild(Entity header) {
	//Todo Evaluator
	return ResultTable(header, pkb.getFollowed());
}

ResultTable NextBipTEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	//Todo Evaluator
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowing(c));
}

ResultTable NextBipTEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	//Todo Evaluator
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowed(c));
}
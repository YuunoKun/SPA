#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "AffectsBipTEvaluator.h"


bool AffectsBipTEvaluator::evaluateWildAndWild() {
	//Todo Evaluator
	return !pkb.isFollowEmpty();
}

bool AffectsBipTEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	//Todo Evaluator
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.isFollow(c1, c2);;
}

bool AffectsBipTEvaluator::evaluateConstantAndWild(Entity e) {
	//Todo Evaluator
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowed(c);
}

bool AffectsBipTEvaluator::evaluateWildAndConstant(Entity e) {
	//Todo Evaluator
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowing(c);
}

ResultTable AffectsBipTEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	//Todo Evaluator
	return ResultTable({ left, right }, pkb.getFollows());
}

ResultTable AffectsBipTEvaluator::evaluateWildAndSynonym(Entity header) {
	//Todo Evaluator
	return ResultTable(header, pkb.getFollowing());
}

ResultTable AffectsBipTEvaluator::evaluateSynonymAndWild(Entity header) {
	//Todo Evaluator
	return ResultTable(header, pkb.getFollowed());
}

ResultTable AffectsBipTEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	//Todo Evaluator
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowing(c));
}

ResultTable AffectsBipTEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	//Todo Evaluator
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowed(c));
}
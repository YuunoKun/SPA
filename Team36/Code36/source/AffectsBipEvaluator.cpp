#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "AffectsBipEvaluator.h"


bool AffectsBipEvaluator::evaluateWildAndWild() {
	//Todo Evaluator
	return !pkb.isFollowEmpty();
}

bool AffectsBipEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	//Todo Evaluator
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.isFollow(c1, c2);;
}

bool AffectsBipEvaluator::evaluateConstantAndWild(Entity e) {
	//Todo Evaluator
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowed(c);
}

bool AffectsBipEvaluator::evaluateWildAndConstant(Entity e) {
	//Todo Evaluator
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowing(c);
}

ResultTable AffectsBipEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	//Todo Evaluator
	return ResultTable({ left, right }, pkb.getFollows());
}

ResultTable AffectsBipEvaluator::evaluateWildAndSynonym(Entity header) {
	//Todo Evaluator
	return ResultTable(header, pkb.getFollowing());
}

ResultTable AffectsBipEvaluator::evaluateSynonymAndWild(Entity header) {
	//Todo Evaluator
	return ResultTable(header, pkb.getFollowed());
}

ResultTable AffectsBipEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	//Todo Evaluator
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowing(c));
}

ResultTable AffectsBipEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	//Todo Evaluator
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowed(c));
}
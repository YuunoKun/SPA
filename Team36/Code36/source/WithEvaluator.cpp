#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "WithEvaluator.h"

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "WithEvaluator.h"

bool WithEvaluator::evaluateWildAndWild() {
	//Todo Evaluator
	return !pkb.isFollowEmpty();
}

bool WithEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	//Todo Evaluator
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.isFollow(c1, c2);;
}

bool WithEvaluator::evaluateConstantAndWild(Entity e) {
	//Todo Evaluator
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowed(c);
}

bool WithEvaluator::evaluateWildAndConstant(Entity e) {
	//Todo Evaluator
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowing(c);
}

ResultTable WithEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	//Todo Evaluator
	return ResultTable({ left, right }, pkb.getFollows());
}

ResultTable WithEvaluator::evaluateWildAndSynonym(Entity header) {
	//Todo Evaluator
	return ResultTable(header, pkb.getFollowing());
}

ResultTable WithEvaluator::evaluateSynonymAndWild(Entity header) {
	//Todo Evaluator
	return ResultTable(header, pkb.getFollowed());
}

ResultTable WithEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	//Todo Evaluator
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowing(c));
}

ResultTable WithEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	//Todo Evaluator
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowed(c));
}
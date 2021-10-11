#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "AffectTEvaluator.h"

bool AffectTEvaluator::haveRelation() {
	//Todo Evaluator
	return !pkb.isFollowEmpty();
}

bool AffectTEvaluator::isRelation(Entity e1, Entity e2) {
	//Todo Evaluator
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.isFollow(c1, c2);;
}

bool AffectTEvaluator::haveRelationAtRight(Entity e) {
	//Todo Evaluator
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowed(c);
}

bool AffectTEvaluator::haveRelationAtLeft(Entity e) {
	//Todo Evaluator
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowing(c);
}

ResultTable AffectTEvaluator::getRelations(Entity left, Entity right) {
	//Todo Evaluator
	return ResultTable({ left, right }, pkb.getFollows());
}

ResultTable AffectTEvaluator::getRightRelations(Entity header) {
	//Todo Evaluator
	return ResultTable(header, pkb.getFollowing());
}

ResultTable AffectTEvaluator::getLeftRelations(Entity header) {
	//Todo Evaluator
	return ResultTable(header, pkb.getFollowed());
}

ResultTable AffectTEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	//Todo Evaluator
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowing(c));
}

ResultTable AffectTEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	//Todo Evaluator
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowed(c));
}
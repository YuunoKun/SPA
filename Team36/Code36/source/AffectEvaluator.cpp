#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "AffectEvaluator.h"


bool AffectEvaluator::haveRelation() {
	//Todo Evaluator
	return !pkb.isFollowEmpty();
}

bool AffectEvaluator::isRelation(Entity e1, Entity e2) {
	//Todo Evaluator
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.isFollow(c1, c2);;
}

bool AffectEvaluator::haveRelationAtRight(Entity e) {
	//Todo Evaluator
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowed(c);
}

bool AffectEvaluator::haveRelationAtLeft(Entity e) {
	//Todo Evaluator
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowing(c);
}

ResultTable AffectEvaluator::getRelations(Entity left, Entity right) {
	//Todo Evaluator
	return ResultTable({ left, right }, pkb.getFollows());
}

ResultTable AffectEvaluator::getRightRelations(Entity header) {
	//Todo Evaluator
	return ResultTable(header, pkb.getFollowing());
}

ResultTable AffectEvaluator::getLeftRelations(Entity header) {
	//Todo Evaluator
	return ResultTable(header, pkb.getFollowed());
}

ResultTable AffectEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	//Todo Evaluator
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowing(c));
}

ResultTable AffectEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	//Todo Evaluator
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowed(c));
}
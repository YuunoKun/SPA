#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "FollowsTEvaluator.h"

bool FollowsTEvaluator::haveRelation() {
	return !pkb.isFollowTEmpty();
}

bool FollowsTEvaluator::isRelation(Entity e1, Entity e2) {
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.isFollowT(c1, c2);
}

bool FollowsTEvaluator::haveRelationAtRight(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowedT(c);
}

bool FollowsTEvaluator::haveRelationAtLeft(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowingT(c);
}

ResultTable FollowsTEvaluator::getRelations(Entity left, Entity right) {
	return ResultTable({ left, right }, pkb.getFollowsT());
}

ResultTable FollowsTEvaluator::getRightRelations(Entity header) {
	return ResultTable(header, pkb.getFollowingT());
}

ResultTable FollowsTEvaluator::getLeftRelations(Entity header) {
	return ResultTable(header, pkb.getFollowedT());
}

ResultTable FollowsTEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowingT(c));
}

ResultTable FollowsTEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowedT(c));
}
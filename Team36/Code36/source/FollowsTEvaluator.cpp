#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "FollowsTEvaluator.h"


//Handle both wild : e.g Relation(_, _)
bool FollowsTEvaluator::haveRelation() {
	return !pkb.isFollowTEmpty();
}

//Handle both constant : e.g Relation(1, 2)
bool FollowsTEvaluator::isRelation(Entity e1, Entity e2) {
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.isFollowT(c1, c2);
}

//Handle left constant, right wild: e.g Relation(1, _)
bool FollowsTEvaluator::haveRelationAtRight(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowedT(c);
}

//Handle right wild, left constant: e.g Relation(_, 1)
bool FollowsTEvaluator::haveRelationAtLeft(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowingT(c);
}

//If both side is declartion: e.g Relation(a, b)
ResultTable FollowsTEvaluator::getRelations(Entity left, Entity right) {
	std::vector<std::pair<StmtInfo, StmtInfo>> results = pkb.getFollowsT();
	std::pair<Entity, Entity> header{ left, right };
	ResultTable result = ResultTable(header, results);
	return result;
}

//If left side is WILD and right side is declartion: e.g Relation(_, a)
ResultTable FollowsTEvaluator::getRightRelations(Entity header) {
	return ResultTable(header, pkb.getFollowingT());
}

//Handle right declartion, left constant: e.g Relation(a, _)
ResultTable FollowsTEvaluator::getLeftRelations(Entity header) {
	return ResultTable(header, pkb.getFollowedT());
}

//Handle left constant, right declartion: e.g Relation(1, a)
ResultTable FollowsTEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowingT(c));
}

//Handle right declartion, left constant: e.g Relation(a, 1)
ResultTable FollowsTEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowedT(c));
}

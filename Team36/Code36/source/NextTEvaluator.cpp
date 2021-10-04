#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "NextTEvaluator.h"

//Handle both wild : e.g Relation(_, _)
bool NextTEvaluator::haveRelation() {
	//Todo Evaluator
	return !pkb.isFollowEmpty();
}

//Handle both constant : e.g Relation(1, 2)
bool NextTEvaluator::isRelation(Entity e1, Entity e2) {
	//Todo Evaluator
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.isFollow(c1, c2);;
}

//Handle left constant, right wild: e.g Relation(1, _)
bool NextTEvaluator::haveRelationAtRight(Entity e) {
	//Todo Evaluator
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowed(c);
}

//Handle right wild, left constant: e.g Relation(_, 1)
bool NextTEvaluator::haveRelationAtLeft(Entity e) {
	//Todo Evaluator
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowing(c);
}

//If both side is declartion: e.g Relation(a, b)
ResultTable NextTEvaluator::getRelations(Entity left, Entity right) {
	//Todo Evaluator
	std::vector<std::pair<StmtInfo, StmtInfo>> results = pkb.getFollows();
	std::pair<Entity, Entity> header{ left, right };
	ResultTable result = ResultTable(header, results);
	return result;
}

//If left side is WILD and right side is declartion: e.g Relation(_, a)
ResultTable NextTEvaluator::getRightRelations(Entity header) {
	//Todo Evaluator
	return ResultTable(header, pkb.getFollowing());
}

//Handle right declartion, left constant: e.g Relation(a, _)
ResultTable NextTEvaluator::getLeftRelations(Entity header) {
	//Todo Evaluator
	return ResultTable(header, pkb.getFollowed());
}

//Handle left constant, right declartion: e.g Relation(1, a)
ResultTable NextTEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	//Todo Evaluator
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowing(c));
}

//Handle right declartion, left constant: e.g Relation(a, 1)
ResultTable NextTEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	//Todo Evaluator
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowed(c));
}
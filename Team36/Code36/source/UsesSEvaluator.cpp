#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "UsesSEvaluator.h"


//Handle both wild : e.g Relation(_, _)
bool UsesSEvaluator::haveRelation() {
	throw std::invalid_argument("haveRelation(): Wild is not allowed for first argument of Uses_S");
}

//Handle both constant : e.g Relation(1, 2)
bool UsesSEvaluator::isRelation(Entity e1, Entity e2) {
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.isFollow(c1, c2);;
}

//Handle left constant, right wild: e.g Relation(1, _)
bool UsesSEvaluator::haveRelationAtRight(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isFollowed(c);;
}

//Handle right wild, left constant: e.g Relation(_, 1)
bool UsesSEvaluator::haveRelationAtLeft(Entity e) {
	throw std::invalid_argument("haveRelationAtLeft(): Wild is not allowed for first argument of Uses_S");
}

//If both side is declartion: e.g Relation(a, b)
ResultTable UsesSEvaluator::getRelations(Entity left, Entity right) {
	std::vector<std::vector<Stmt>> results = pkb.getFollows();
	std::vector<Entity> header{ left, right };
	ResultTable result = ResultTable(header, results);
	return result;
}

//If left side is WILD and right side is declartion: e.g Relation(_, a)
ResultTable UsesSEvaluator::getRightRelations(Entity header) {
	throw std::invalid_argument("getRightRelations(): Wild is not allowed for first argument of Uses_S");
}

//Handle right declartion, left constant: e.g Relation(a, _)
ResultTable UsesSEvaluator::getLeftRelations(Entity header) {
	return ResultTable(header, pkb.getFollowed());
}

//Handle left constant, right declartion: e.g Relation(1, a)
ResultTable UsesSEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowing(c));
}

//Handle right declartion, left constant: e.g Relation(a, 1)
ResultTable UsesSEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getFollowed(c));
}

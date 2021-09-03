#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "ParentTEvaluator.h"



//Handle both wild : e.g Relation(_, _)
bool ParentTEvaluator::haveRelation() {
	return pkb.getParentT().empty();
}

//Handle both constant : e.g Relation(1, 2)
bool ParentTEvaluator::isRelation(Entity e1, Entity e2) {
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.isParentT(c1, c2);;
}

//Handle left constant, right wild: e.g Relation(1, _)
bool ParentTEvaluator::haveRelationAtRight(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isParentT(c);;
}

//Handle right wild, left constant: e.g Relation(_, 1)
bool ParentTEvaluator::haveRelationAtLeft(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isChildT(c);;
}

//If both side is declartion: e.g Follows(a, b)
ResultTable ParentTEvaluator::getRelations(Entity left, Entity right) {
	std::vector<std::vector<Stmt>> results = pkb.getAllParentRelation();
	std::vector<Entity> header{ left, right };
	ResultTable result = ResultTable(header, results);
	return result;
}

//If left side is WILD and right side is declartion: e.g Follows(_, a)
ResultTable ParentTEvaluator::getRightRelations(Entity header) {
	return ResultTable(header, pkb.getChildT());
}

//Handle right declartion, left constant: e.g Relation(a, _)
ResultTable ParentTEvaluator::getLeftRelations(Entity header) {
	return ResultTable(header, pkb.getParentT());
}

//Handle left constant, right declartion: e.g Relation(1, a)
ResultTable ParentTEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getChildT(c));
}

//Handle right declartion, left constant: e.g Relation(a, 1)
ResultTable ParentTEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getParentT(c));
}
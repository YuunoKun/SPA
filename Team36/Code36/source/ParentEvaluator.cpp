#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "ParentEvaluator.h"


//Handle both wild : e.g Relation(_, _)
bool ParentEvaluator::haveRelation() {
	return !pkb.isParentEmpty();
}

//Handle both constant : e.g Relation(1, 2)
bool ParentEvaluator::isRelation(Entity e1, Entity e2) {
	stmt_index c1 = stoi(e1.getValue());
	stmt_index c2 = stoi(e2.getValue());
	return pkb.isParent(c1, c2);
}

//Handle left constant, right wild: e.g Relation(1, _)
bool ParentEvaluator::haveRelationAtRight(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isParent(c);
}

//Handle right wild, left constant: e.g Relation(_, 1)
bool ParentEvaluator::haveRelationAtLeft(Entity e) {
	stmt_index c = stoi(e.getValue());
	return pkb.isChild(c);
}

//If both side is declartion: e.g Relation(a, b)
ResultTable ParentEvaluator::getRelations(Entity left, Entity right) {
	std::vector<std::pair<StmtInfo, StmtInfo>> results = pkb.getAllParentRelation();
	std::pair<Entity, Entity> header{ left, right };
	ResultTable result = ResultTable(header, results);
	return result;
}

//If left side is WILD and right side is declartion: e.g Relation(_, a)
ResultTable ParentEvaluator::getRightRelations(Entity header) {
	return ResultTable(header, pkb.getChild());
}

//Handle right declartion, left constant: e.g Relation(a, _)
ResultTable ParentEvaluator::getLeftRelations(Entity header) {
	return ResultTable(header, pkb.getParent());
}

//Handle left constant, right declartion: e.g Relation(1, a)
ResultTable ParentEvaluator::getRelationMatchLeft(Entity constant, Entity header) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getChild(c));
}

//Handle right declartion, left constant: e.g Relation(a, 1)
ResultTable ParentEvaluator::getRelationMatchRight(Entity header, Entity constant) {
	stmt_index c = stoi(constant.getValue());
	return ResultTable(header, pkb.getParent(c));
}

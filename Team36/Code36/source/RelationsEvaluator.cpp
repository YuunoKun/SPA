#include "RelationsEvaluator.h"

#include "Utility.h"

void RelationsEvaluator::evaluateRelation(QueryResult& result, RelRef& relation) {
	/*
	switch (relation.getType()) {
	case FOLLOWS: evaluateFollowsRelation(result, relation); break;
	case FOLLOWS_T: evaluateFollowsTRelation(result, relation); break;
	case PARENT: evaluateParentRelation(result, relation); break;
	case PARENT_T: evaluateParentTRelation(result, relation); break;
	case MODIFIES_P: evaluateModifiesRelation(result, relation); break;
	case USES_P: evaluateUsesRelation(result, relation); break;
	default: throw std::domain_error("Some Relation is not being handle!!!!");
	}
	*/
}

void RelationsEvaluator::evaluateRelation(QueryResult& queryResult, RelRef& relation, RelationEvaluatorInterface& evaluator) {
	Entity clauses1 = relation.getClauses()[0];
	Entity clauses2 = relation.getClauses()[1];

	//if there is at least one side is declaration
	if (clauses1.isDeclaration() || clauses2.isDeclaration()) {
		ResultTable* resultTable;
		if (clauses1.isDeclaration() && clauses2.isDeclaration()) {
			//If both side is declartion: e.g Follows(a, b)
			//Get all follow relation
			resultTable = & evaluator.getRelations(clauses1, clauses2);
		}else if (clauses1.isDeclaration() && clauses2.getType() == WILD) {
			//If left side is declaration and right side is WILD: e.g Follows(a, _)
			resultTable = &evaluator.getLeftRelations(clauses1);

		}
		else if (clauses1.getType() == WILD && clauses2.isDeclaration()) {
			//If left side is WILD and right side is declartion: e.g Follows(_, a)
			resultTable = &evaluator.getRightRelations(clauses2);
		}
		else if (clauses1.isDeclaration() && !clauses2.isDeclaration()) {
			//If left side is declaration and right side is constant: e.g Follows(a, 1)
			resultTable = &evaluator.getRelationMatchRight(clauses1, clauses2);
		}
		else if (!clauses1.isDeclaration() && clauses2.isDeclaration()) {
			//If left side is constant and right side is declartion: e.g Follows(1, a)
			//get all statment that is following of specfic index
			resultTable = &evaluator.getRelationMatchLeft(clauses1, clauses2);
		}
		else {
			throw std::out_of_range("Some Follow logic have not be handle for one side declaration");
		}

		if (resultTable->isEmpty()) {
			queryResult.setNoResult();
		}
		else {
			queryResult.addResult(*resultTable);
		}
		
	}else {
		//Handle both side NOT declartion
		bool haveResult = false;

		if (clauses2.getType() == WILD && clauses2.getType() == WILD) {
			//If both side is WILD: e.g Follows(_, _)
			haveResult = !evaluator.haveRelation();
		}
		else if (!clauses1.isDeclaration() && clauses2.getType() == WILD) {
			//If left side is constant and right side is WILD: e.g Follows(1, _)
			haveResult = !evaluator.haveRelationAtRight(clauses1);
		}
		else if (clauses1.getType() == WILD && !clauses2.isDeclaration()) {
			//If left side is WILD and right side is Constant: e.g Follows(_, 1)
			haveResult = !evaluator.haveRelationAtLeft(clauses2);;
		}
		else if (!clauses1.isDeclaration() && !clauses2.isDeclaration()) {
			//both is constant, check if relation exist e.g Follows(1, 2)
			haveResult = !evaluator.isRelation(clauses1, clauses2);
		}
		else {
			throw std::out_of_range("Some Follow logic have not be handle for both side none declaration");
		}

		if (!haveResult) {
			queryResult.setNoResult();
		}
	}
}

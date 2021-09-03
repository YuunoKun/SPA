#include "RelationEvaluator.h"

#include "Utility.h"

void RelationEvaluator::evaluateRelation(QueryResult& result, RelRef& relation) {
	switch (relation.getType()) {
	case MODIFIES_P: evaluateModifiesRelation(result, relation); break;
	case USES_P: evaluateUsesRelation(result, relation); break;
	case PARENT: evaluateParentRelation(result, relation); break;
	case PARENT_T: evaluateParentTRelation(result, relation); break;
	case FOLLOWS: evaluateFollowsRelation(result, relation); break;
	case FOLLOWS_T: evaluateFollowsTRelation(result, relation); break;
	}
}
void RelationEvaluator::evaluateModifiesRelation(QueryResult& result, RelRef& relation) {
}

void RelationEvaluator::evaluateUsesRelation(QueryResult& result, RelRef& relation) {
}

void RelationEvaluator::evaluateParentRelation(QueryResult& result, RelRef& relation) {
}

void RelationEvaluator::evaluateParentTRelation(QueryResult& result, RelRef& relation) {
}

void RelationEvaluator::evaluateFollowsRelation(QueryResult& queryResult, RelRef& relation) {
	Entity clauses1 = relation.getClauses()[0];
	Entity clauses2 = relation.getClauses()[1];

	if (!clauses2.isDeclaration() && clauses2.isDeclaration()) {
		//If both side is declartion: e.g Follows(a, b)
		//Check if relation exist
		std::vector<std::vector<Stmt>> results = pkb.getFollows();
		if (results.empty()) {
			queryResult.setNoResult();
		}
		else {
			queryResult.addResult({ relation.getClauses(), results });
		}
	}
	else if (!clauses1.isDeclaration() && !clauses2.isDeclaration()) {
		//Handle both side NOT declartion
		bool haveResult = false;

		if (clauses2.getType() == WILD && clauses2.getType() == WILD) {
			//If both side is WILD: e.g Follows(_, _)
			haveResult = !pkb.getFollows().empty();

		}
		else if (!clauses1.isDeclaration() && clauses2.getType() == WILD) {
			//If left side is constant and right side is WILD: e.g Follows(1, _)
			//Check if relation exist
			stmt_index c1 = stoi(clauses1.getValue());
			haveResult = pkb.isFollowed(c1);
		}
		else if (clauses1.getType() == WILD && !clauses2.isDeclaration()) {
			//If left side is WILD and right side is Constant: e.g Follows(_, 1)
			//Check if relation exist
			stmt_index c2 = stoi(clauses2.getValue());
			haveResult = pkb.isFollowing(c2);

		}
		else if (!clauses1.isDeclaration() && !clauses2.isDeclaration()) {
			//both is constant, check if relation exist e.g Follows(1, 2)
			stmt_index c1 = stoi(clauses1.getValue());
			stmt_index c2 = stoi(clauses2.getValue());
			haveResult = pkb.isFollow(c1, c2);
		}
		else {
			throw std::out_of_range("Some FollowT logic have not be handle for both side none declaration");
		}

		if (!haveResult) {
			queryResult.setNoResult();
		}
	} else {
		//Handle one side declaration
		Entity declartion;
		std::vector<Stmt> result;
		if (clauses1.isDeclaration() && clauses2.getType() == WILD) {
			//If left side is declaration and right side is WILD: e.g Follows(a, _)
			//get all statment that have follow
			declartion = clauses1;
			result = pkb.getFollowed();

		}
		else if (clauses1.getType() == WILD && clauses2.isDeclaration()) {
			//If left side is WILD and right side is declartion: e.g Follows(_, a)
			//get all statment that have follow
			declartion = clauses2;
			result = pkb.getFollowingT();
		}
		else if (clauses1.isDeclaration() && !clauses2.isDeclaration()) {
			//If left side is declaration and right side is constant: e.g Follows(a, 1)
			//get all statment that have follow
			declartion = clauses1;

			stmt_index c2 = stoi(clauses2.getValue());
			std::vector<Stmt> result = pkb.getFollowedT(c2);
		}
		else if (!clauses1.isDeclaration() && clauses2.isDeclaration()) {
			//If left side is constant and right side is declartion: e.g Follows(1, a)
			//Check if relation exist
			declartion = clauses2;

			stmt_index c1 = stoi(clauses1.getValue());
			std::vector<Stmt> result = pkb.getFollowingT(c1);
		}
		else {
			throw std::out_of_range("Some FollowT logic have not be handle for one side declaration");
		}

		if (result.empty()) {
			queryResult.setNoResult();
		}
		else {
			ResultTable table({ declartion, result });
			queryResult.addResult(table);
		}
	}
	
}

void RelationEvaluator::evaluateFollowsTRelation(QueryResult& queryResult, RelRef& relation) {

	Entity clauses1 = relation.getClauses()[0];
	Entity clauses2 = relation.getClauses()[1];
	if (!clauses2.isDeclaration() && clauses2.isDeclaration()) {
		//If both side is declartion: e.g Follows*(a, b)
		//Check if relation exist
		std::vector<std::vector<Stmt>> results = pkb.getFollowsT();
		if (results.empty()) {
			queryResult.setNoResult();
		}
		else {
			queryResult.addResult({ relation.getClauses(), results });
		}
	}
	else if(!clauses1.isDeclaration() && !clauses2.isDeclaration()) {
		//Handle both side NOT declartion
		bool haveResult = false;

			if (clauses2.getType() == WILD && clauses2.getType() == WILD) {
				//If both side is WILD: e.g Follows*(_, _)
				haveResult = !pkb.getFollowsT().empty();

			}else if (!clauses1.isDeclaration() && clauses2.getType() == WILD) {
				//If left side is constant and right side is WILD: e.g Follows*(1, _)
				//Check if relation exist
				stmt_index c1 = stoi(clauses1.getValue());
				haveResult = pkb.isFollowedT(c1);
			}
			else if (clauses1.getType() == WILD && !clauses2.isDeclaration()) {
				//If left side is WILD and right side is Constant: e.g Follows*(_, 1)
				//Check if relation exist
				stmt_index c2 = stoi(clauses2.getValue());
				haveResult = pkb.isFollowingT(c2);

			}else if (!clauses1.isDeclaration() && !clauses2.isDeclaration()) {
				//both is constant, check if relation exist e.g Follows*(1, 2)
				stmt_index c1 = stoi(clauses1.getValue());
				stmt_index c2 = stoi(clauses2.getValue());
				haveResult = pkb.isFollowT(c1, c2);
			}else {
				throw std::out_of_range("Some FollowT logic have not be handle for both side none declaration");
			}
			if (!haveResult) {
				queryResult.setNoResult();
			}
	}else {
		//Handle one side declaration
		Entity declartion;
		std::vector<Stmt> result;
		if (clauses1.isDeclaration() && clauses2.getType() == WILD) {
			//If left side is declaration and right side is WILD: e.g Follows*(a, _)
			//get all statment that have follow
			declartion = clauses1;
			result = pkb.getFollowedT();

		}
		else if (clauses1.getType() == WILD && clauses2.isDeclaration()) {
			//If left side is WILD and right side is declartion: e.g Follows*(_, a)
			//get all statment that have follow
			declartion = clauses2;
			result = pkb.getFollowingT();
		}
		else if (clauses1.isDeclaration() && !clauses2.isDeclaration()) {
			//If left side is declaration and right side is constant: e.g Follows*(a, 1)
			//get all statment that have follow
			declartion = clauses1;

			stmt_index c2 = stoi(clauses2.getValue());
			std::vector<Stmt> result = pkb.getFollowedT(c2);
		}
		else if (!clauses1.isDeclaration() && clauses2.isDeclaration()) {
			//If left side is constant and right side is declartion: e.g Follows*(1, a)
			//Check if relation exist
			declartion = clauses2;

			stmt_index c1 = stoi(clauses1.getValue());
			std::vector<Stmt> result = pkb.getFollowingT(c1);
		}
		else {
			throw std::out_of_range("Some FollowT logic have not be handle for one side declaration");
		}

		if (result.empty()) {
			queryResult.setNoResult();
		}else {
			ResultTable table({ declartion, result });
			queryResult.addResult(table);
		}
	}
}

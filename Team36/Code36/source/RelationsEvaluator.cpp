#include "RelationsEvaluator.h"

#include "Utility.h"

void RelationsEvaluator::evaluateRelation(QueryResult& result, RelRef& relation) {
	switch (relation.getType()) {
	case FOLLOWS: evaluateFollowsRelation(result, relation); break;
	case FOLLOWS_T: evaluateFollowsTRelation(result, relation); break;
	case PARENT: evaluateParentRelation(result, relation); break;
	case PARENT_T: evaluateParentTRelation(result, relation); break;
	case MODIFIES_P: evaluateModifiesRelation(result, relation); break;
	case USES_P: evaluateUsesRelation(result, relation); break;
	default: throw std::domain_error("Some Relation is not being handle!!!!");
	}
}


void RelationsEvaluator::evaluateFollowsRelation(QueryResult& queryResult, RelRef& relation) {
	if (relation.getType() != FOLLOWS) {
		throw std::invalid_argument("Wrong relation to evaluate! this is FOLLOWS");
	}
	Entity clauses1 = relation.getClauses()[0];
	Entity clauses2 = relation.getClauses()[1];

	if (clauses1.isDeclaration() && clauses2.isDeclaration()) {
		//If both side is declartion: e.g Follows(a, b)
		//Get all follow relation
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
			//Check if there exist any follow
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
			throw std::out_of_range("Some Follow logic have not be handle for both side none declaration");
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
			//get all statment that is following
			declartion = clauses2;
			result = pkb.getFollowingT();
		}
		else if (clauses1.isDeclaration() && !clauses2.isDeclaration()) {
			//If left side is declaration and right side is constant: e.g Follows(a, 1)
			//get all statment that have follow of specfic index
			declartion = clauses1;

			stmt_index c2 = stoi(clauses2.getValue());
			std::vector<Stmt> result = pkb.getFollowedT(c2);
		}
		else if (!clauses1.isDeclaration() && clauses2.isDeclaration()) {
			//If left side is constant and right side is declartion: e.g Follows(1, a)
			//get all statment that is following of specfic index
			declartion = clauses2;

			stmt_index c1 = stoi(clauses1.getValue());
			std::vector<Stmt> result = pkb.getFollowingT(c1);
		}
		else {
			throw std::out_of_range("Some Follow logic have not be handle for one side declaration");
		}

		if (result.empty()) {
			queryResult.setNoResult();
		}else {
			ResultTable table({ declartion, result });
			queryResult.addResult(table);
		}
	}
}

void RelationsEvaluator::evaluateFollowsTRelation(QueryResult& queryResult, RelRef& relation) {
	if (relation.getType() != FOLLOWS_T) {
		throw std::invalid_argument("Wrong relation to evaluate! this is FOLLOWS_T");
	}
	Entity clauses1 = relation.getClauses()[0];
	Entity clauses2 = relation.getClauses()[1];
	if (!clauses2.isDeclaration() && clauses2.isDeclaration()) {
		//If both side is declartion: e.g Follows*(a, b)
		//Get all followT relation
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
				//Check if there exist any followT
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
			//get all statment that have followT
			declartion = clauses1;
			result = pkb.getFollowedT();

		}
		else if (clauses1.getType() == WILD && clauses2.isDeclaration()) {
			//If left side is WILD and right side is declartion: e.g Follows*(_, a)
			//get all statment that is followingT
			declartion = clauses2;
			result = pkb.getFollowingT();
		}
		else if (clauses1.isDeclaration() && !clauses2.isDeclaration()) {
			//If left side is declaration and right side is constant: e.g Follows*(a, 1)
			//get all statment that have followT at specfic index
			declartion = clauses1;

			stmt_index c2 = stoi(clauses2.getValue());
			std::vector<Stmt> result = pkb.getFollowedT(c2);
		}
		else if (!clauses1.isDeclaration() && clauses2.isDeclaration()) {
			//If left side is constant and right side is declartion: e.g Follows*(1, a)
			//get all statment that is followingT at specfic index
			declartion = clauses2;

			stmt_index c1 = stoi(clauses1.getValue());
			std::vector<Stmt> result = pkb.getFollowingT(c1);
		}else {
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

void RelationsEvaluator::evaluateParentRelation(QueryResult& queryResult, RelRef& relation) {

	if (relation.getType() != PARENT) {
		throw std::invalid_argument("Wrong relation to evaluate! this is PARENT");
	}
	Entity clauses1 = relation.getClauses()[0];
	Entity clauses2 = relation.getClauses()[1];

	if (clauses1.isDeclaration() && clauses2.isDeclaration()) {
		//If both side is declartion: e.g Follows(a, b)
		//Get all follow relation
		std::vector<std::vector<Stmt>> results = pkb.getAllParent();
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
			//Check if there exist any follow
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
			throw std::out_of_range("Some Follow logic have not be handle for both side none declaration");
		}

		if (!haveResult) {
			queryResult.setNoResult();
		}
	}
	else {
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
			//get all statment that is following
			declartion = clauses2;
			result = pkb.getFollowingT();
		}
		else if (clauses1.isDeclaration() && !clauses2.isDeclaration()) {
			//If left side is declaration and right side is constant: e.g Follows(a, 1)
			//get all statment that have follow of specfic index
			declartion = clauses1;

			stmt_index c2 = stoi(clauses2.getValue());
			std::vector<Stmt> result = pkb.getFollowedT(c2);
		}
		else if (!clauses1.isDeclaration() && clauses2.isDeclaration()) {
			//If left side is constant and right side is declartion: e.g Follows(1, a)
			//get all statment that is following of specfic index
			declartion = clauses2;

			stmt_index c1 = stoi(clauses1.getValue());
			std::vector<Stmt> result = pkb.getFollowingT(c1);
		}
		else {
			throw std::out_of_range("Some Follow logic have not be handle for one side declaration");
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

void RelationsEvaluator::evaluateParentTRelation(QueryResult& result, RelRef& relation) {
}

void RelationsEvaluator::evaluateModifiesRelation(QueryResult& result, RelRef& relation) {
}

void RelationsEvaluator::evaluateUsesRelation(QueryResult& result, RelRef& relation) {
}

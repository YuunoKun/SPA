#include "RelationsEvaluator.h"
#include "Utility.h"
#include "FollowsEvaluator.h"
#include "FollowsTEvaluator.h"
#include "ParentEvaluator.h"
#include "ParentTEvaluator.h"
#include "ModifiesPEvaluator.h"
#include "ModifiesSEvaluator.h"
#include "UsesPEvaluator.h"
#include "UsesSEvaluator.h"
#include "WithEvaluator.h"
#include "CallsEvaluator.h"
#include "CallsTEvaluator.h"
#include "NextEvaluator.h"
#include "NextTEvaluator.h"
#include "AffectsEvaluator.h"
#include "AffectsTEvaluator.h"
#include "NextBipEvaluator.h"
#include "NextBipTEvaluator.h"
#include "AffectsBipEvaluator.h"
#include "AffectsBipTEvaluator.h"

void RelationsEvaluator::evaluateRelation(QueryResult& result, RelRef& relation) {
	switch (relation.getType()) {
	case RelType::FOLLOWS: evaluateRelation(result, relation, FollowsEvaluator()); break;
	case RelType::FOLLOWS_T: evaluateRelation(result, relation, FollowsTEvaluator()); break;
	case RelType::PARENT: evaluateRelation(result, relation, ParentEvaluator()); break;
	case RelType::PARENT_T: evaluateRelation(result, relation, ParentTEvaluator()); break;
	case RelType::MODIFIES_P: evaluateRelation(result, relation, ModifiesPEvaluator()); break;
	case RelType::MODIFIES_S: evaluateRelation(result, relation, ModifiesSEvaluator()); break;
	case RelType::USES_P: evaluateRelation(result, relation, UsesPEvaluator()); break;
	case RelType::USES_S: evaluateRelation(result, relation, UsesSEvaluator()); break;
	case RelType::WITH: evaluateRelation(result, relation, WithEvaluator()); break;
	case RelType::CALLS: evaluateRelation(result, relation, CallsEvaluator()); break;
	case RelType::CALLS_T: evaluateRelation(result, relation, CallsTEvaluator()); break;
	case RelType::NEXT: evaluateRelation(result, relation, NextEvaluator()); break;
	case RelType::NEXT_T: evaluateRelation(result, relation, NextTEvaluator()); break;
	case RelType::AFFECT: evaluateRelation(result, relation, AffectsEvaluator()); break;
	case RelType::AFFECT_T: evaluateRelation(result, relation, AffectsTEvaluator()); break;
	case RelType::NEXT_BIP: evaluateRelation(result, relation, NextBipEvaluator()); break;
	case RelType::NEXT_BIP_T: evaluateRelation(result, relation, NextBipTEvaluator()); break;
	case RelType::AFFECT_BIP_T: evaluateRelation(result, relation, AffectsBipTEvaluator()); break;
	case RelType::AFFECT_BIP: evaluateRelation(result, relation, AffectsBipEvaluator()); break;
	default: throw std::domain_error("Some Relation is not being handle!!!!");
	}
}

void RelationsEvaluator::evaluateRelation(QueryResult& query_result, RelRef& relation, RelationEvaluatorInterface& evaluator) {
	Entity clauses1 = relation.getFirstClause();
	Entity clauses2 = relation.getSecondClause();

	if (clauses1.isSynonym() || clauses2.isSynonym()) {
		//if there is at least one side is synonym, merge table generated
		if (clauses1.isSynonym() && clauses2.isSynonym()) {
			query_result.addResult(evaluator.evaluateSynonymAndSynonym(clauses1, clauses2));
		} else if (clauses1.isSynonym() && clauses2.getType() == EntityType::WILD) {
			ResultTable resultTable = evaluator.evaluateSynonymAndWild(clauses1);
			query_result.addResult(evaluator.evaluateSynonymAndWild(clauses1));
		} else if (clauses1.getType() == EntityType::WILD && clauses2.isSynonym()) {
			query_result.addResult(evaluator.evaluateWildAndSynonym(clauses2));
		} else if (clauses1.isSynonym() && !clauses2.isSynonym()) {
			query_result.addResult(evaluator.evaluateSynonymAndConstant(clauses1, clauses2));
		} else if (!clauses1.isSynonym() && clauses2.isSynonym()) {
			query_result.addResult(evaluator.evaluateConstantAndSynonym(clauses1, clauses2));
		} else {
			throw std::out_of_range("Some Follow logic have not be handle for synonym");
		}
	} else {
		//check if result exist for relation with no synonym
		bool haveResult = false;
		if (clauses1.getType() == EntityType::WILD && clauses2.getType() == EntityType::WILD) {
			haveResult = evaluator.evaluateWildAndWild();
		} else if (!clauses1.isSynonym() && clauses2.getType() == EntityType::WILD) {
			haveResult = evaluator.evaluateConstantAndWild(clauses1);
		} else if (clauses1.getType() == EntityType::WILD && !clauses2.isSynonym()) {
			haveResult = evaluator.evaluateWildAndConstant(clauses2);;
		} else if (!clauses1.isSynonym() && !clauses2.isSynonym()) {
			haveResult = evaluator.evaluateConstantAndConstant(clauses1, clauses2);
		} else {
			throw std::out_of_range("Some Follow logic have not be handle for both side no synonym");
		}

		if (!haveResult) {
			query_result.setNoResult();
		}
	}
}
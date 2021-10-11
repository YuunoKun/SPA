#include "PatternsEvaluator.h"

#include "Utility.h"
#include "PatternAssignEvaluator.h"
#include "PatternIfEvaluator.h"
#include "PatternWhileEvaluator.h"

void PatternsEvaluator::evaluatePattern(QueryResult& queryResult, Pattern& pattern) {
	switch (pattern.getPatternType().getType()) {
	case EntityType::ASSIGN:  return evaluatePattern(queryResult, pattern, PatternAssignEvaluator());
		break;
	case EntityType::IF: return evaluatePattern(queryResult, pattern, PatternIfEvaluator());
		break;
	case EntityType::WHILE: return evaluatePattern(queryResult, pattern, PatternWhileEvaluator());
		break;
	default:
		throw std::invalid_argument("evaluatePattern(): only does not support this type");
	}
}



void PatternsEvaluator::evaluatePattern(QueryResult& queryResult, Pattern& pattern, PatternEvaluatorInterface& evaluator) {
	Entity lhsEntity = pattern.getLeftExpression();
	if (lhsEntity.isSynonym()) {
		queryResult.addResult(evaluator.evaluateSynonym(pattern));
	}
	else if(lhsEntity.getType() == EntityType::WILD) {
		queryResult.addResult(evaluator.evaluateWild(pattern));
	}
	else {
		queryResult.addResult(evaluator.evaluateConstant(pattern));
	}
}

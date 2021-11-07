#include "PatternsEvaluator.h"
#include "PQL/Helper/Utility.h"
#include "PatternEvaluator/PatternAssignEvaluator.h"
#include "PatternEvaluator/PatternIfEvaluator.h"
#include "PatternEvaluator/PatternWhileEvaluator.h"

void PatternsEvaluator::evaluatePattern(QueryResult& query_result, Pattern& pattern) {
	switch (pattern.getPatternType().getType()) {
	case EntityType::ASSIGN:  return evaluatePattern(query_result, pattern, PatternAssignEvaluator());
		break;
	case EntityType::IF: return evaluatePattern(query_result, pattern, PatternIfEvaluator());
		break;
	case EntityType::WHILE: return evaluatePattern(query_result, pattern, PatternWhileEvaluator());
		break;
	default:
		throw std::invalid_argument("evaluatePattern(): only does not support this type");
	}
}

void PatternsEvaluator::evaluatePattern(QueryResult& query_result, Pattern& pattern, PatternEvaluatorInterface& evaluator) {
	Entity lhs_entity = pattern.getLeftExpression();
	if (lhs_entity.isSynonym()) {
		query_result.addResult(evaluator.evaluateSynonym(pattern));
	} else if (lhs_entity.getType() == EntityType::WILD) {
		query_result.addResult(evaluator.evaluateWild(pattern));
	} else {
		query_result.addResult(evaluator.evaluateConstant(pattern));
	}
}
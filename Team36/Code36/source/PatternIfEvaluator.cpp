#include "PatternIfEvaluator.h"
#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"

ResultTable PatternIfEvaluator::evaluateSynonym(Pattern& pattern) {
	return ResultTable({ pattern.getPatternType(), pattern.getLeftExpression() }, pkb.getAllIfUses());
}

ResultTable PatternIfEvaluator::evaluateWild(Pattern& pattern) {
	return ResultTable(pattern.getPatternType(), pkb.getIfUses());
}

ResultTable PatternIfEvaluator::evaluateConstant(Pattern& pattern) {
	return ResultTable(pattern.getPatternType(), pkb.getIfUses(pattern.getLeftExpression().getValue()));
}
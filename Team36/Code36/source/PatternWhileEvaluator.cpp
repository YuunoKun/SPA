#include "Pattern.h"
#include "ResultTable.h"
#include "PKBAdapter.h"
#include "PatternWhileEvaluator.h"

ResultTable PatternWhileEvaluator::evaluateSynonym(Pattern& pattern) {
	return ResultTable({ pattern.getPatternType(), pattern.getLeftExpression() }, pkb.getAllWhileUses());
}

ResultTable PatternWhileEvaluator::evaluateWild(Pattern& pattern) {
	return ResultTable(pattern.getPatternType(), pkb.getWhileUses());
}

ResultTable PatternWhileEvaluator::evaluateConstant(Pattern& pattern) {
	return ResultTable(pattern.getPatternType(), pkb.getWhileUses(pattern.getLeftExpression().getValue()));
}
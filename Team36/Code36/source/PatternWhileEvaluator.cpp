
#include "Pattern.h"
#include "ResultTable.h"
#include "PKBAdapter.h"
#include "PatternWhileEvaluator.h"

ResultTable PatternWhileEvaluator::evaluateSynonym(Pattern& pattern) {
    std::pair<Entity, Entity> header = { pattern.getPatternType(), pattern.getLeftExpression()};
    return ResultTable(header, pkb.getAllWhileUses());
}

ResultTable PatternWhileEvaluator::evaluateWild(Pattern& pattern) {
    Entity header = pattern.getPatternType();
    return ResultTable(header, pkb.getWhileUses());
}

ResultTable PatternWhileEvaluator::evaluateConstant(Pattern& pattern) {
    Entity header = pattern.getPatternType();
    return ResultTable(header, pkb.getWhileUses(pattern.getLeftExpression().getValue()));
}

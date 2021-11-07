#pragma once

#include "PatternEvaluatorInterface.h"

class PatternAssignEvaluator : public PatternEvaluatorInterface {
public:
	ResultTable evaluateSynonym(Pattern&) override;
	ResultTable evaluateWild(Pattern&) override;
	ResultTable evaluateConstant(Pattern&) override;
protected:
	bool isExpressionWild(Pattern&);
};
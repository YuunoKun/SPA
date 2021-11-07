#pragma once

#include "PatternEvaluatorInterface.h"

class PatternIfEvaluator : public PatternEvaluatorInterface {
public:
	ResultTable evaluateSynonym(Pattern&) override;
	ResultTable evaluateWild(Pattern&) override;
	ResultTable evaluateConstant(Pattern&) override;
};
#pragma once

#include "PatternEvaluator/PatternEvaluatorInterface.h"

class PatternsEvaluator {
public:
	void evaluatePattern(QueryResult&, Pattern&);
private:
	void evaluatePattern(QueryResult&, Pattern&, PatternEvaluatorInterface&);
};
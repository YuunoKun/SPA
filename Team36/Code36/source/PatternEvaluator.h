#pragma once

#include "Query.h"
#include "QueryResult.h"

class PatternEvaluator {
public:
	QueryResult evaluatePattern(Query, Pattern);
};
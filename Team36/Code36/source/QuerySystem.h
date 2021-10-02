#pragma once

#include "Query.h"
#include "QueryPreProcessor.h"
#include "QueryOptimizer.h"
#include "QueryEvaluator.h"
#include "ResultProjector.h"

class QuerySystem {
public:
	std::list<std::string> processQuery(std::string);
};

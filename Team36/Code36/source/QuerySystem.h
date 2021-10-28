#pragma once

#include "Query.h"
#include "QueryPreProcessor.h"
#include "QueryOptimizer.h"
#include "QueryEvaluator.h"

class QuerySystem {
public:
	std::list<std::string> processQuery(std::string);
private:
	std::list<std::string> returnEmptyResult(Entity&);
};

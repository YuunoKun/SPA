#pragma once
#include <list>
#include "Clause.h"

class QueryOptimizer {
public:
	static std::vector<Clause> optimizeClausesOrder(std::vector<Clause>&);
};
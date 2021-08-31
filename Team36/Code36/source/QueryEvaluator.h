#pragma once

#include "Query.h"
#include "PKBAdapter.h"

class QueryEvaluator {
public:
	QueryEvaluator();
	QueryEvaluator(PKBAdapter pkb);

	std::string evaluateQuery(Query);

private:
	void initEntity(Query&);
	void initEntityConstants(Entity*, std::vector<constant>);
	void initEntityProcedures(Entity*, std::vector<procedure_name>);
	void initEntityStmts(Entity*, std::vector<Stmt>);
	void initEntityVariables(Entity*, std::vector<variable_name>);

	std::string getResult(Query&);

	PKBAdapter pkb;
};

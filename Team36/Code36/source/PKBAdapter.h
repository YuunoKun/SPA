#pragma once


class PKBAdapter {
public:
	PKBAdapter();

	std::vector<variable_name> getVariables();
	std::vector<Stmt> getStmts();
	std::vector<constant> getConstants();
	std::vector<procedure_name> getProcedures();
};

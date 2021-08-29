#pragma once

#include<stdio.h>

#include <iostream>
#include <string>
#include <unordered_set>

#include "Common.h"
#include "Stmt.h"

typedef short PROC;

class TNode;

class PKB {
public:
	static int setProcToAST(PROC p, TNode* r);
	static TNode* getRootAST(PROC p);

	void setProcedures(std::vector<procedure_name> proc_set);
	void setConstants(std::vector<constant> const_set);
	void setVariables(std::vector<variable_name> var_set);
	void setStmts(std::vector<Stmt> stmt_set);

	std::vector<procedure_name> getProcedures();
	std::vector<constant> getConstants();
	std::vector<variable_name> getVariables();
	std::vector<Stmt> getStmts();

private:
	std::unordered_set<procedure_name> proc_table;
	std::unordered_set<constant> const_table;
	std::unordered_set<variable_name> var_table;

	/* To add hash function for Stmt */
	//std::unordered_set<Stmt> stmt_table;
};
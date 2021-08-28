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
	std::unordered_set<procedure_name> proc_table;
	std::unordered_set<constant> const_table;
	std::unordered_set<variable_name> var_table;
	std::unordered_set<Stmt> stmt_table;

public:
	static int setProcToAST(PROC p, TNode* r);
	static TNode* getRootAST(PROC p);

	void setProcedures(vector<procedure_name> proc_set);
	void setConstants(vector<constant> const_set);
	void setVariables(vector<variable_name> var_set);
	void setStmts(vector<Stmt> stmt_set);

	vector<procedure_name> getProcedures();
	vector<constant> getConstants();
	vector<variable_name> getVariables();
	vector<Stmt> getStmts();
};
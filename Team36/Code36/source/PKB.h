#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include "../SPA/Common.h"
#include "../SPA/Stmt.h"

using namespace std;
typedef short PROC;

class TNode;

class PKB {
	unordered_set<variable> var_table;
	unordered_set<procedure> proc_table;
	unordered_set<Stmt> stmt_table;
	unordered_set<constant> const_table;
public:
	static int setProcToAST(PROC p, TNode* r);
	static TNode* getRootAST(PROC p);

	void setProcedures(vector<procedure> proc_set);
	void setConstants(vector<constant> const_set);
	void setVariables(vector<variable> var_set);
	void setStmts(vector<Stmt> stmt_set);

	vector<procedure> getProcedures();
	vector<constant> getConstants();
	vector<variable> getVariables();
	vector<Stmt> getStmts();
};
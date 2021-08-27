#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"
#include "TNode.h"

int PKB::setProcToAST(PROC p, TNode* r) {
	return 0;
}

TNode* PKB::getRootAST(PROC p) {
	return nullptr;
}

void PKB::setProcedures(vector<procedure> proc_set) {
	std::copy(proc_set.begin(), proc_set.end(), std::inserter(proc_table, proc_table.end()));
}

void PKB::setConstants(vector<constant> const_set) {
	std::copy(const_set.begin(), const_set.end(), std::inserter(const_table, const_table.end()));
}

void PKB::setVariables(vector<variable> var_set) {
	std::copy(var_set.begin(), var_set.end(), std::inserter(var_table, var_table.end()));
}

void PKB::setStmts(vector<Stmt> stmt_set) {
	//std::copy(stmt_set.begin(), stmt_set.end(), std::inserter(stmt_table, stmt_table.end()));
}

vector<variable> PKB::getVariables()
{
	return vector<variable>(var_table.begin(), var_table.end());
}

vector<Stmt> PKB::getStmts()
{
	return vector<Stmt>(stmt_table.begin(), stmt_table.end());
}

vector<constant> PKB::getConstants()
{
	return vector<constant>(const_table.begin(), const_table.end());
}

vector<procedure> PKB::getProcedures()
{
	return vector<procedure>(proc_table.begin(), proc_table.end());
}
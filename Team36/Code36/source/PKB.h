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
	static PKB& PKB::getInstance();
	PKB(PKB const&) = delete;
	void operator=(PKB const&) = delete;

	void setProcedures(std::vector<proc_name> proc_set);
	void setConstants(std::vector<constant> const_set);
	void setVariables(std::vector<var_name> var_set);
	void setStmts(std::vector<Stmt> stmt_set);

	std::vector<proc_name> getProcedures();
	std::vector<constant> getConstants();
	std::vector<var_name> getVariables();
	std::vector<Stmt> getStmts();

	void resetCache();
	void resetEntities();
	void resetRelations();

private:
	std::unordered_set<proc_name> proc_table;
	std::unordered_set<constant> const_table;
	std::unordered_set<var_name> var_table;

	/* To add hash function for Stmt */
	//std::unordered_set<Stmt> stmt_table;

	PKB() {};
};
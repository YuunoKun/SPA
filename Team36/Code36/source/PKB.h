#pragma once

#include<stdio.h>

#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "Common.h"
#include "Stmt.h"

typedef short PROC;

class Procedure {};
class Variable {};

class PKB {
public:
	static PKB& PKB::getInstance();
	PKB(PKB const&) = delete;
	void operator=(PKB const&) = delete;

	/*	proc_index addProcedure(Procedure& procedure);
		var_index addVariable(Variable& variable);
		stmt_index addStmt(Stmt& statement);*/
	void addConstant(constant constant);
	void addProcedures(proc_name proc_name);
	void addVariable(var_name var_name);
	void addStmt(StmtType stmt_type);
	void addExprTree(stmt_index stmt_index, std::string expr);

	void addParent(stmt_index parent, stmt_index child);
	void addParentT(stmt_index parent, stmt_index child);
	void addFollows(stmt_index followed, stmt_index follower);
	void addFollowsT(stmt_index followed, stmt_index follower);
	void addUses(stmt_index user, var_name used);
	void addModifies(stmt_index modifier, var_name modified);

	void setProcedures(std::vector<proc_name> proc_set);
	void setConstants(std::vector<constant> const_set);
	void setVariables(std::vector<var_name> var_set);

	std::vector<proc_name> getProcedures();
	std::vector<constant> getConstants();
	std::vector<var_name> getVariables();
	std::vector<Stmt> getStmts();

	void resetCache();
	void resetEntities();

private:
	std::unordered_set<proc_name> old_proc_table;
	std::unordered_set<var_name> old_var_table;
	std::unordered_set<constant> const_table;
	std::unordered_map<proc_index, proc_name> proc_table;
	std::unordered_map<var_index, var_name> var_table;
	std::unordered_set<stmt_index> stmt_table;
	std::unordered_set<stmt_index> assign_table;
	std::unordered_map<var_index, std::string> assignment_table;
	std::unordered_map<stmt_index, stmt_index> follows_table;
	std::unordered_multimap<stmt_index, stmt_index> parents_table;
	std::unordered_multimap<stmt_index, stmt_index> followsT_table;
	std::unordered_multimap<stmt_index, stmt_index> parentsT_table;
	std::unordered_multimap<stmt_index, stmt_index> usesS_table;
	std::unordered_map<stmt_index, stmt_index> modifiesS_table;

	PKB() {};
};
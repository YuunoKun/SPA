#include<stdio.h>

#include <iostream>
#include <string>

#include "PKB.h"
#include "TNode.h"

PKB& PKB::getInstance() {
	static PKB pkb;
	return pkb;
}

//proc_index PKB::addProcedure(Procedure& procedure)
//{
//	int proc_index = proc_table.size() + 1;
//	proc_table.emplace(proc_index, procedure);
//	return proc_index;
//}
//
//var_index PKB::addVariable(Variable& variable)
//{
//	int var_index = var_table.size() + 1;
//	proc_table.emplace(var_index, variable);
//	return var_index;
//}

//stmt_index PKB::addStmt(Stmt& statement)
//{
//	int stmt_index = stmt_table.size() + 1;
//	stmt_table.emplace(stmt_index, statement);
//	addStmtType(statement.getNum(), statement.getType());
//	return stmt_index;
//}

void PKB::addConstant(constant constant)
{
	const_table.emplace(constant);
	return;
}

void PKB::addParent(stmt_index parent, stmt_index child)
{
	parents_table.emplace(parent, child);
}

void PKB::addParentT(stmt_index parent, stmt_index child)
{
	auto iter = parentsT_table.find(parent);
	if (iter != parentsT_table.end() && iter->second != child)
		// only add if key-value pair is unique
	{
		parentsT_table.emplace(parent, child);
	}
}

void PKB::addFollows(stmt_index followed, stmt_index follower)
{
	follows_table.emplace(followed, follower);
}

void PKB::addFollowsT(stmt_index followed, stmt_index follower)
{
	auto iter = parentsT_table.find(followed);
	if (iter != parentsT_table.end() && iter->second != follower)
		// only add if key-value pair is unique
	{
		parentsT_table.emplace(followed, follower);
	}
}

void PKB::resetCache()
{
	old_proc_table.clear();
	const_table.clear();
	old_var_table.clear();
}

void PKB::resetEntities()
{
	old_proc_table.clear();
	const_table.clear();
	old_var_table.clear();
}

void PKB::setProcedures(std::vector<proc_name> proc_set) {
	std::copy(proc_set.begin(), proc_set.end(), std::inserter(old_proc_table, old_proc_table.end()));
}

void PKB::setConstants(std::vector<constant> const_set) {
	std::copy(const_set.begin(), const_set.end(), std::inserter(const_table, const_table.end()));
}

void PKB::setVariables(std::vector<var_name> var_set) {
	std::copy(var_set.begin(), var_set.end(), std::inserter(old_var_table, old_var_table.end()));
}

std::vector<var_name> PKB::getVariables() {
	return std::vector<var_name>(old_var_table.begin(), old_var_table.end());
}

std::vector<Stmt> PKB::getStmts() {
	//return std::vector<Stmt>(stmt_table.begin(), stmt_table.end());    --- To add hash function for Stmt
	return std::vector<Stmt>();
}

std::vector<constant> PKB::getConstants() {
	return std::vector<constant>(const_table.begin(), const_table.end());
}

std::vector<proc_name> PKB::getProcedures() {
	return std::vector<proc_name>(old_proc_table.begin(), old_proc_table.end());
}
#include<stdio.h>

#include <iostream>
#include <string>

#include "PKB.h"
#include "TNode.h"

PKB& PKB::getInstance() {
	static PKB pkb;
	return pkb;
}

void PKB::resetCache()
{
	proc_table.clear();
	const_table.clear();
	var_table.clear();
}

void PKB::resetEntities()
{
	proc_table.clear();
	const_table.clear();
	var_table.clear();
}

void PKB::resetRelations()
{
}

void PKB::setProcedures(std::vector<procedure_name> proc_set) {
	std::copy(proc_set.begin(), proc_set.end(), std::inserter(proc_table, proc_table.end()));
}

void PKB::setConstants(std::vector<constant> const_set) {
	std::copy(const_set.begin(), const_set.end(), std::inserter(const_table, const_table.end()));
}

void PKB::setVariables(std::vector<variable_name> var_set) {
	std::copy(var_set.begin(), var_set.end(), std::inserter(var_table, var_table.end()));
}

void PKB::setStmts(std::vector<Stmt> stmt_set) {
	//std::copy(stmt_set.begin(), stmt_set.end(), std::inserter(stmt_table, stmt_table.end()));
}

std::vector<variable_name> PKB::getVariables() {
	return std::vector<variable_name>(var_table.begin(), var_table.end());
}

std::vector<Stmt> PKB::getStmts() {
	//return std::vector<Stmt>(stmt_table.begin(), stmt_table.end());    --- To add hash function for Stmt
	return std::vector<Stmt>();
}

std::vector<constant> PKB::getConstants() {
	return std::vector<constant>(const_table.begin(), const_table.end());
}

std::vector<procedure_name> PKB::getProcedures() {
	return std::vector<procedure_name>(proc_table.begin(), proc_table.end());
}
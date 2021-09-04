#include<stdio.h>

#include <iostream>
#include <string>

#include "PKB.h"
#include "TNode.h"
#include "RelationTable.h"
// need to include this .cpp for template classes
#include "RelationTable.cpp"

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

void PKB::addProcedures(proc_name proc_name)
{
	for (auto const& element : proc_table) {
		if (element == proc_name) {
			return;
		}
	};
	proc_table.push_back(proc_name);
}

void PKB::addVariable(var_name var_name)
{
	for (auto const& element : var_table) {
		if (element == var_name) {
			return;
		}
	};
	proc_table.push_back(var_name);
}

void PKB::addStmt(StmtType stmt_type)
{
	stmt_index stmt_index = stmt_table.size() + 1;
	StmtInfo new_stmt_info{ stmt_index, stmt_type };
	stmt_table.push_back(new_stmt_info);
}

void PKB::addExprTree(var_index var_index, std::string expr)
{
	expr_table.emplace(var_index, expr);
}

void PKB::addParent(stmt_index parent, stmt_index child)
{
	try {
		StmtInfo parent_stmt_info{ parent, stmt_table.at(parent).stmt_type };
		StmtInfo child_stmt_info{ child, stmt_table.at(child).stmt_type };
		parent_table.insert(parent_stmt_info, child_stmt_info);
	}
	catch (std::out_of_range& e) {
		throw std::invalid_argument("addParent: Invalid stmt indexes: [" + std::to_string(parent)
			+ "," + std::to_string(child) + "]");
	}
}

void PKB::addFollows(stmt_index first, stmt_index second)
{
	try {
		StmtInfo first_stmt_info{ first, stmt_table.at(first).stmt_type };
		StmtInfo second_stmt_info{ second, stmt_table.at(second).stmt_type };
		parent_table.insert(second_stmt_info, second_stmt_info);
	}
	catch (std::out_of_range& e) {
		throw std::invalid_argument("addFollows: Invalid stmt indexes: [" + std::to_string(first)
			+ "," + std::to_string(second) + "]");
	}
}

void PKB::addUsesS(stmt_index user, var_name used)
{
}

void PKB::addModifiesS(stmt_index modifier, var_name modified)
{
}

void PKB::generateParentT()
{
	parentT_table = parent_table.findTransitiveClosure();
}

void PKB::generateFollowT()
{
	followsT_table = followsT_table.findTransitiveClosure();
}

void PKB::resetCache()
{
	const_table.clear();
	proc_table.clear();
	var_table.clear();
	stmt_table.clear();
	const_table.clear();
	assignment_table.clear();
	expr_table.clear();
	follows_table.clear();
	parent_table.clear();
	followsT_table.clear();
	parentT_table.clear();
	usesS_table.clear();
	modifiesS_table.clear();
	follows_reverse_table.clear();
	parent_reverse_table.clear();
	followsT_reverse_table.clear();
	parentT_reverse_table.clear();
	usesS_reverse_table.clear();
	modifiesS_reverse_table.clear();

	old_proc_table.clear();
	old_var_table.clear();
}

void PKB::resetEntities()
{
	const_table.clear();
	proc_table.clear();
	stmt_table.clear();
	const_table.clear();

	old_proc_table.clear();
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

const std::vector<proc_name>& PKB::get_procedures()
{
	return proc_table;
}

const std::vector<var_name>& PKB::get_variables()
{
	return var_table;
}

const std::vector<StmtInfo>& PKB::get_stmts()
{
	return stmt_table;
}

const std::unordered_set<constant>& PKB::get_consts()
{
	return const_table;
}

const std::unordered_map<stmt_index, var_index> PKB::get_assignment_table()
{
	return assignment_table;
}

const std::unordered_map<var_index, std::string> PKB::get_expr_table()
{
	return expr_table;
}

const UniqueRelationTable<StmtInfo, StmtInfo>& PKB::get_follows_table()
{
	return follows_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::get_parent_table()
{
	return parent_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::get_followsT_table()
{
	return followsT_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::get_parentT_table()
{
	return parentT_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::get_usesS_table()
{
	return usesS_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::get_modifiesS_table()
{
	return modifiesS_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::get_follows_reverse_table()
{
	return followsT_reverse_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::get_parent_reverse_table()
{
	return parent_reverse_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::get_followsT_reverse_table()
{
	return followsT_reverse_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::get_parentT_reverse_table()
{
	return parentT_reverse_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::get_usesS_reverse_table()
{
	return usesS_reverse_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::get_modifiesS_reverse_table()
{
	return modifiesS_reverse_table;
}

std::vector<constant> PKB::getConstants() {
	return std::vector<constant>(const_table.begin(), const_table.end());
}

std::vector<proc_name> PKB::getProcedures() {
	return std::vector<proc_name>(old_proc_table.begin(), old_proc_table.end());
}
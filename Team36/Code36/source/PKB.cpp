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

void PKB::addConstant(constant constant)
{
	const_table.emplace(constant);
	return;
}

void PKB::addProcedure(proc_name proc_name)
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
	var_table.push_back(var_name);
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
		StmtInfo parent_stmt_info{ parent, stmt_table.at(parent - 1).stmt_type };
		StmtInfo child_stmt_info{ child, stmt_table.at(child - 1).stmt_type };
		parent_table.insert(parent_stmt_info, child_stmt_info);
	}
	catch (std::out_of_range&) {
		throw std::invalid_argument("addParent: Invalid stmt indexes: [" + std::to_string(parent)
			+ "," + std::to_string(child) + "]");
	}
}

void PKB::addFollows(stmt_index first, stmt_index second)
{
	try {
		StmtInfo first_stmt_info{ first, stmt_table.at(first - 1).stmt_type };
		StmtInfo second_stmt_info{ second, stmt_table.at(second - 1).stmt_type };
		follows_table.insert(first_stmt_info, second_stmt_info);
	}
	catch (std::out_of_range&) {
		throw std::invalid_argument("addFollows: Invalid stmt indexes: [" + std::to_string(first)
			+ "," + std::to_string(second) + "]");
	}
}

void PKB::addUsesS(stmt_index user, var_name used)
{
	try {
		StmtInfo user_stmt_info{ user, stmt_table.at(user - 1).stmt_type };
		std::vector<var_name>::iterator it = std::find(var_table.begin(), var_table.end(), used);
		if (it != var_table.end()) {
			usesS_table.insert(user_stmt_info, used);
		}
		else {
			throw std::invalid_argument("addUsesS: Invalid var name: " + used);
		}
	}
	catch (std::out_of_range&) {
		throw std::invalid_argument("addUsesS: Invalid stmt index:" + std::to_string(user));
	}
}

void PKB::addModifiesS(stmt_index modifier, var_name modified)
{
	try {
		StmtInfo modifier_stmt_info{ modifier, stmt_table.at(modifier - 1).stmt_type };
		std::vector<var_name>::iterator it = std::find(var_table.begin(), var_table.end(), modified);
		if (it != var_table.end()) {
			modifiesS_table.insert(modifier_stmt_info, modified);
		}
		else {
			throw std::invalid_argument("addModifiesS: Invalid var name: " + modified);
		}
	}
	catch (std::out_of_range&) {
		throw std::invalid_argument("addModifiesS: Invalid stmt index:" + std::to_string(modifier));
	}
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
}

void PKB::resetEntities()
{
	const_table.clear();
	proc_table.clear();
	stmt_table.clear();
	const_table.clear();
}

const std::vector<proc_name>& PKB::getProcedures()
{
	return proc_table;
}

const std::vector<var_name>& PKB::getVariables()
{
	return var_table;
}

const std::vector<StmtInfo>& PKB::getStmts()
{
	return stmt_table;
}

const std::vector<constant> PKB::getConstants()
{
	std::vector<constant> v(const_table.begin(), const_table.end());
	return v;
}

const std::unordered_map<stmt_index, var_index>& PKB::getAssigns()
{
	return assignment_table;
}

const std::unordered_map<var_index, std::string>& PKB::getExpr()
{
	return expr_table;
}

const UniqueRelationTable<StmtInfo, StmtInfo>& PKB::getFollows()
{
	return follows_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::getParent()
{
	return parent_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::getFollowsT()
{
	return followsT_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::getParentT()
{
	return parentT_table;
}

const RelationTable<StmtInfo, var_name>& PKB::getUsesS()
{
	return usesS_table;
}

const RelationTable<StmtInfo, var_name>& PKB::getModifiesS()
{
	return modifiesS_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::getFollowsReverse()
{
	if (follows_reverse_table.empty()) {
		follows_reverse_table = follows_table.findReverse();
	}
	return follows_reverse_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::getParentReverse()
{
	if (parent_reverse_table.empty()) {
		parent_reverse_table = parent_table.findReverse();
	}
	return parent_reverse_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::getFollowsTReverse()
{
	if (followsT_reverse_table.empty()) {
		followsT_reverse_table = followsT_table.findReverse();
	}
	return followsT_reverse_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::getParentTReverse()
{
	if (parentT_reverse_table.empty()) {
		parentT_reverse_table = parentT_table.findReverse();
	}
	return parentT_reverse_table;
}

const RelationTable<var_name, StmtInfo>& PKB::getUsesSReverse()
{
	if (usesS_reverse_table.empty()) {
		usesS_reverse_table = usesS_table.findReverse();
	}
	return usesS_reverse_table;
}

const RelationTable<var_name, StmtInfo>& PKB::getModifiesSReverse()
{
	if (modifiesS_reverse_table.empty()) {
		modifiesS_reverse_table = modifiesS_table.findReverse();
	}
	return modifiesS_reverse_table;
}
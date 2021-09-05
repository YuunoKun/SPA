#pragma once

#include<stdio.h>

#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "Common.h"
#include "Stmt.h"
#include "RelationTable.h"

class PKB {
public:
	static PKB& PKB::getInstance();
	PKB(PKB const&) = delete;
	void operator=(PKB const&) = delete;

	void addConstant(constant constant);
	void addProcedures(proc_name proc_name);
	void addVariable(var_name var_name);
	void addStmt(StmtType stmt_type);
	void addExprTree(var_index var_index, std::string expr);

	void addParent(stmt_index parent, stmt_index child);
	void addFollows(stmt_index first, stmt_index second);
	void addUsesS(stmt_index user, var_name used);
	void addModifiesS(stmt_index modifier, var_name modified);
	void generateParentT();
	void generateFollowT();

	const std::vector<proc_name>& get_procedures();
	const std::vector<var_name>& get_variables();
	const std::vector<StmtInfo>& get_stmts();
	const std::unordered_set<constant>& get_consts();
	const std::unordered_map<stmt_index, var_index> get_assignment_table();
	const std::unordered_map<var_index, std::string> get_expr_table();
	const UniqueRelationTable<StmtInfo, StmtInfo>& get_follows_table();
	const RelationTable<StmtInfo, StmtInfo>& get_parent_table();
	const RelationTable<StmtInfo, StmtInfo>& get_followsT_table();
	const RelationTable<StmtInfo, StmtInfo>& get_parentT_table();
	const RelationTable<StmtInfo, var_name>& get_usesS_table();
	const RelationTable<StmtInfo, var_name>& get_modifiesS_table();
	const RelationTable<StmtInfo, StmtInfo>& get_follows_reverse_table();
	const RelationTable<StmtInfo, StmtInfo>& get_parent_reverse_table();
	const RelationTable<StmtInfo, StmtInfo>& get_followsT_reverse_table();
	const RelationTable<StmtInfo, StmtInfo>& get_parentT_reverse_table();
	const RelationTable<var_name, StmtInfo>& get_usesS_reverse_table();
	const RelationTable<var_name, StmtInfo>& get_modifiesS_reverse_table();

	void resetCache();
	void resetEntities();

	// Old methods, to be deleted
	void setProcedures(std::vector<proc_name> proc_set);
	void setConstants(std::vector<constant> const_set);
	void setVariables(std::vector<var_name> var_set);

	std::vector<proc_name> getProcedures();
	std::vector<constant> getConstants();
	std::vector<var_name> getVariables();
	std::vector<Stmt> getStmts();

private:
	std::vector<proc_name> proc_table;
	std::vector<var_name> var_table;
	std::vector<StmtInfo> stmt_table;
	std::unordered_set<constant> const_table;
	std::unordered_map<stmt_index, var_index> assignment_table;
	std::unordered_map<var_index, std::string> expr_table;
	UniqueRelationTable<StmtInfo, StmtInfo> follows_table;
	RelationTable<StmtInfo, StmtInfo> parent_table;
	RelationTable<StmtInfo, StmtInfo> followsT_table;
	RelationTable<StmtInfo, StmtInfo> parentT_table;
	RelationTable<StmtInfo, var_name> usesS_table;
	RelationTable<StmtInfo, var_name> modifiesS_table;
	RelationTable<StmtInfo, StmtInfo> follows_reverse_table;
	RelationTable<StmtInfo, StmtInfo> parent_reverse_table;
	RelationTable<StmtInfo, StmtInfo> followsT_reverse_table;
	RelationTable<StmtInfo, StmtInfo> parentT_reverse_table;
	RelationTable<var_name, StmtInfo> usesS_reverse_table;
	RelationTable<var_name, StmtInfo> modifiesS_reverse_table;

	// Old tables, to be deleted
	std::unordered_set<proc_name> old_proc_table;
	std::unordered_set<var_name> old_var_table;

	PKB() {};
};
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
	void addProcedure(proc_name proc_name);
	void addVariable(var_name var_name);
	void addStmt(StmtType stmt_type);
	void addExprTree(stmt_index stmt_index, expr expr);

	void addParent(stmt_index parent, stmt_index child);
	void addFollows(stmt_index first, stmt_index second);
	void addUsesS(stmt_index user, var_name used);
	void addModifiesS(stmt_index modifier, var_name modified);
	void generateParentT();
	void generateFollowT();

	const std::vector<proc_name>& getProcedures();
	const std::vector<var_name>& getVariables();
	const std::vector<StmtInfo>& getStmts();
	const StmtInfo getStmt(stmt_index stmt_index);
	const var_name getAssignment(stmt_index stmt_index);
	const expr getExpression(stmt_index stmt_index);
	const std::vector<constant> getConstants();
	const UniqueRelationTable<stmt_index, var_name>& getAssigns();
	const UniqueRelationTable<stmt_index, expr>& getExpr();
	const UniqueRelationTable<StmtInfo, StmtInfo>& getFollows();
	const RelationTable<StmtInfo, StmtInfo>& getParent();
	const RelationTable<StmtInfo, StmtInfo>& getFollowsT();
	const RelationTable<StmtInfo, StmtInfo>& getParentT();
	const RelationTable<StmtInfo, var_name>& getUsesS();
	const RelationTable<StmtInfo, var_name>& getModifiesS();

	void resetCache();
	void resetEntities();

private:
	std::vector<proc_name> proc_table;
	std::vector<var_name> var_table;
	std::vector<StmtInfo> stmt_table;
	std::unordered_set<constant> const_table;
	UniqueRelationTable<stmt_index, var_name> assignment_table;
	UniqueRelationTable<stmt_index, expr> expr_table;
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

	PKB() {};
};
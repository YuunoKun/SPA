#pragma once

#include<stdio.h>

#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "Common.h"
#include "KnowledgeBase.h"
#include "RelationTable.h"

class PKB : public KnowledgeBase {
public:
	static PKB& PKB::getInstance();
	PKB(PKB const&) = delete;
	void operator=(PKB const&) = delete;

	void addConstant(constant constant) override;
	void addProcedure(proc_name proc_name) override;
	void addVariable(var_name var_name) override;
	void addStmt(StmtType stmt_type) override;
	void addExprTree(stmt_index stmt_index, expr expr) override;

	void addParent(stmt_index parent, stmt_index child) override;
	void addFollows(stmt_index first, stmt_index second) override;
	void addUsesS(stmt_index user, var_name used) override;
	void addModifiesS(stmt_index modifier, var_name modified) override;
	void addModifiesP(proc_name proc, var_name modified) override;
	void addUsesP(proc_name proc, var_name used) override;

	void generateParentT() override;
	void generateFollowsT() override;

	const std::vector<proc_name>& getProcedures() override;
	const std::vector<var_name>& getVariables() override;
	const std::vector<StmtInfo>& getStmts() override;
	const StmtInfo getStmt(stmt_index stmt_index) override;
	const var_name getAssignment(stmt_index stmt_index) override;
	const expr getExpression(stmt_index stmt_index) override;
	const std::vector<constant> getConstants() override;
	const UniqueRelationTable<stmt_index, var_name>& getAssigns();
	const UniqueRelationTable<stmt_index, expr>& getExpr() override;
	const UniqueRelationTable<StmtInfo, StmtInfo>& getFollows() override;
	const RelationTable<StmtInfo, StmtInfo>& getParent() override;
	const RelationTable<StmtInfo, StmtInfo>& getFollowsT() override;
	const RelationTable<StmtInfo, StmtInfo>& getParentT() override;
	const RelationTable<StmtInfo, var_name>& getUsesS() override;
	const RelationTable<StmtInfo, var_name>& getModifiesS() override;
	const RelationTable<proc_name, var_name>& getUsesP() override;
	const RelationTable<proc_name, var_name>& getModifiesP() override;

	void resetCache() override;
	void resetEntities() override;

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
	RelationTable<proc_name, var_name> usesP_table;
	RelationTable<proc_name, var_name> modifiesP_table;

	PKB() {};
};
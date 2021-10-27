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
#include "MonotypeRelationTable.h"

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
	void addCallsP(proc_name caller_proc_name, proc_name callee_proc_name) override;
	void addCallsS(stmt_index caller_stmt_index, proc_name callee_proc_name) override;
	void addIf(stmt_index if_stmt_index, var_name control_var) override;
	void addWhile(stmt_index while_stmt_index, var_name control_var) override;
	void addNext(prog_line prog_line1, prog_line prog_line2) override;
	void addProcContains(proc_name proc, stmt_index index) override;

	void generateParentT() override;
	void generateFollowsT() override;
	void generateCallsPT() override;

	const std::vector<proc_name>& getProcedures() override;
	const std::vector<var_name>& getVariables() override;
	const std::vector<StmtInfo>& getStmts() override;
	const std::unordered_map<stmt_index, expr>& getExpr() override;
	const StmtInfo getStmt(stmt_index stmt_index) override;
	const var_name getAssignment(stmt_index stmt_index) override;
	expr getExpression(stmt_index stmt_index) override;
	const std::vector<constant> getConstants() override;
	const RelationTable<stmt_index, var_name>& getAssigns();
	const MonotypeRelationTable<StmtInfo>& getFollows() override;
	const MonotypeRelationTable<StmtInfo>& getParent() override;
	const MonotypeRelationTable<StmtInfo>& getFollowsT() override;
	const MonotypeRelationTable<StmtInfo>& getParentT() override;
	const RelationTable<StmtInfo, var_name>& getUsesS() override;
	const RelationTable<StmtInfo, var_name>& getModifiesS() override;
	const RelationTable<proc_name, var_name>& getUsesP() override;
	const RelationTable<proc_name, var_name>& getModifiesP() override;
	const MonotypeRelationTable<proc_name>& getCallsP() override;
	const MonotypeRelationTable<proc_name>& getCallsPT() override;
	const RelationTable<stmt_index, var_name>& getRead() override;
	const RelationTable<stmt_index, var_name>& getPrint() override;
	const RelationTable<stmt_index, proc_name>& getCallsS() override;
	const RelationTable<stmt_index, var_name>& getIf() override;
	const RelationTable<stmt_index, var_name>& getWhile() override;
	const MonotypeRelationTable<StmtInfo>& getNext() override;
	const RelationTable<proc_name, stmt_index>& getProcContains() override;

	void resetCache() override;
	void resetEntities() override;

private:
	std::vector<proc_name> proc_table;
	std::vector<var_name> var_table;
	std::vector<StmtInfo> stmt_table;
	std::unordered_set<constant> const_table;
	std::unordered_map<stmt_index, expr> expr_table;
	RelationTable<stmt_index, var_name> assignment_table;
	MonotypeRelationTable<StmtInfo> follows_table;
	MonotypeRelationTable<StmtInfo> parent_table;
	MonotypeRelationTable<StmtInfo> followsT_table;
	MonotypeRelationTable<StmtInfo> parentT_table;
	RelationTable<StmtInfo, var_name> usesS_table;
	RelationTable<StmtInfo, var_name> modifiesS_table;
	RelationTable<proc_name, var_name> usesP_table;
	RelationTable<proc_name, var_name> modifiesP_table;
	MonotypeRelationTable<proc_name> callsP_table;
	MonotypeRelationTable<proc_name> callsPT_table;
	RelationTable<stmt_index, var_name> read_table;
	RelationTable<stmt_index, var_name> print_table;
	RelationTable<stmt_index, proc_name> callsS_table;
	RelationTable<stmt_index, var_name> while_table;
	RelationTable<stmt_index, var_name> if_table;
	MonotypeRelationTable<StmtInfo> next_table;
	RelationTable<proc_name, stmt_index> procS_table;

	PKB() {};
};
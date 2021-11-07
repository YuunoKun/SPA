#pragma once

#include<stdio.h>

#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "PKBSourceInterface.h"
#include "PKBQueryInterface.h"

class PKB : public PKBSourceInterface, public PKBQueryInterface {
public:
	static PKB& PKB::getInstance();
	PKB(PKB const&) = delete;
	void operator=(PKB const&) = delete;
	~PKB();

	void addConstant(constant constant);
	void addProcedure(proc_name proc_name);
	void addVariable(var_name var_name);
	void addStmt(StmtType stmt_type);
	void addExprTree(stmt_index stmt_index, expr expr);

	void addParent(stmt_index parent, stmt_index child);
	void addFollows(stmt_index first, stmt_index second);
	void addUsesS(stmt_index user, var_name used);
	void addModifiesS(stmt_index modifier, var_name modified);
	void addModifiesP(proc_name proc, var_name modified);
	void addUsesP(proc_name proc, var_name used);
	void addCallsP(proc_name caller_proc_name, proc_name callee_proc_name);
	void addCallsS(stmt_index caller_stmt_index, proc_name callee_proc_name);
	void addIf(stmt_index if_stmt_index, var_name control_var);
	void addWhile(stmt_index while_stmt_index, var_name control_var);
	void addNext(prog_line prog_line1, prog_line prog_line2);
	void addProcContains(proc_name proc, stmt_index index);
	void addCFGsToDestroy(std::vector<CFG*>);
	void addCFGBip(CFG*);

	void generateParentT();
	void generateFollowsT();
	void generateCallsPT();

	const std::vector<StmtInfo>& getStmts();
	const std::unordered_map<stmt_index, expr>& getExpr();
	const StmtInfo getStmt(stmt_index stmt_index);
	const var_name getAssignment(stmt_index stmt_index);
	expr getExpression(stmt_index stmt_index);
	const std::vector<proc_name> getProcedures();
	const std::vector<var_name> getVariables();
	const std::vector<constant> getConstants();
	const std::unordered_set<proc_name>& getProceduresSet();
	const std::unordered_set<var_name>& getVariablesSet();
	const std::unordered_set<constant>& getConstantsSet();
	const RelationTable<stmt_index, var_name>& getAssigns();
	const MonotypeRelationTable<StmtInfo>& getFollows();
	const MonotypeRelationTable<StmtInfo>& getParent();
	const MonotypeRelationTable<StmtInfo>& getFollowsT();
	const MonotypeRelationTable<StmtInfo>& getParentT();
	const RelationTable<StmtInfo, var_name>& getUsesS();
	const RelationTable<StmtInfo, var_name>& getModifiesS();
	const RelationTable<proc_name, var_name>& getUsesP();
	const RelationTable<proc_name, var_name>& getModifiesP();
	const MonotypeRelationTable<proc_name>& getCallsP();
	const MonotypeRelationTable<proc_name>& getCallsPT();
	const RelationTable<stmt_index, var_name>& getRead();
	const RelationTable<stmt_index, var_name>& getPrint();
	const RelationTable<stmt_index, proc_name>& getCallsS();
	const RelationTable<stmt_index, var_name>& getIf();
	const RelationTable<stmt_index, var_name>& getWhile();
	const MonotypeRelationTable<StmtInfo>& getNext();
	const RelationTable<proc_name, stmt_index>& getProcContains();
	std::vector<CFG*> getCFGBips();

	void resetCache();
	void resetEntities();

private:
	std::vector<StmtInfo> stmt_table{};
	std::unordered_set<proc_name> proc_table{};
	std::unordered_set<var_name> var_table{};
	std::unordered_set<constant> const_table{};
	std::unordered_map<stmt_index, expr> expr_table{};
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
	std::vector<CFG*> cfgs_to_destroy{};
	std::vector<CFG*> cfg_bips{};

	PKB() {};
};
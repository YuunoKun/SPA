#pragma once

#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "Common.h"
#include "Token.h"
#include "PKB.h"
#include "RelationTable.h"

class KnowledgeBase {
public:
	virtual void addConstant(constant constant) = 0;
	virtual void addProcedure(proc_name proc_name) = 0;
	virtual void addVariable(var_name var_name) = 0;
	virtual void addStmt(StmtType stmt_type) = 0;
	virtual void addExprTree(stmt_index stmt_index, expr expr) = 0;

	virtual void addParent(stmt_index parent, stmt_index child) = 0;
	virtual void addFollows(stmt_index first, stmt_index second) = 0;
	virtual void addUsesS(stmt_index user, var_name used) = 0;
	virtual void addModifiesS(stmt_index modifier, var_name modified) = 0;
	virtual void addModifiesP(proc_name proc, var_name modified) = 0;
	virtual void addUsesP(proc_name proc, var_name used) = 0;

	virtual void addNext(prog_line prog_line1, prog_line prog_line2) = 0;
	virtual void addCallsP(proc_name caller_proc_name, proc_name callee_proc_name) = 0;
	virtual void addCallsS(stmt_index caller_stmt_index, proc_name callee_proc_name) = 0;
	virtual void addIf(stmt_index if_stmt_index, var_name control_var) = 0;
	virtual void addWhile(stmt_index while_stmt_index, var_name control_var) = 0;

	virtual void generateParentT() = 0;
	virtual void generateFollowsT() = 0;

	virtual const std::vector<proc_name>& getProcedures() = 0;
	virtual const std::vector<var_name>& getVariables() = 0;
	virtual const std::vector<StmtInfo>& getStmts() = 0;
	virtual const StmtInfo getStmt(stmt_index stmt_index) = 0;
	virtual const var_name getAssignment(stmt_index stmt_index) = 0;
	virtual const expr getExpression(stmt_index stmt_index) = 0;
	virtual const std::vector<constant> getConstants() = 0;
	virtual const UniqueRelationTable<stmt_index, var_name>& getAssigns() = 0;
	virtual const UniqueRelationTable<stmt_index, expr>& getExpr() = 0;
	virtual const UniqueRelationTable<StmtInfo, StmtInfo>& getFollows() = 0;
	virtual const RelationTable<StmtInfo, StmtInfo>& getParent() = 0;
	virtual const RelationTable<StmtInfo, StmtInfo>& getFollowsT() = 0;
	virtual const RelationTable<StmtInfo, StmtInfo>& getParentT() = 0;
	virtual const RelationTable<StmtInfo, var_name>& getUsesS() = 0;
	virtual const RelationTable<StmtInfo, var_name>& getModifiesS() = 0;
	virtual const RelationTable<proc_name, var_name>& getUsesP() = 0;
	virtual const RelationTable<proc_name, var_name>& getModifiesP() = 0;
	virtual const RelationTable<proc_name, proc_name>& getCallsP() = 0;
	virtual const RelationTable<proc_name, proc_name>& getCallsPT() = 0;
	virtual const RelationTable<stmt_index, proc_name>& getCallsS() = 0;
	virtual const UniqueRelationTable<stmt_index, var_name>& getRead() = 0;
	virtual const UniqueRelationTable<stmt_index, var_name>& getPrint() = 0;
	virtual const RelationTable<stmt_index, var_name>& getIf() = 0;
	virtual const RelationTable<stmt_index, var_name>& getWhile() = 0;
	virtual const RelationTable<StmtInfo, StmtInfo>& getNext() = 0;

	virtual void resetCache() = 0;
	virtual void resetEntities() = 0;
};
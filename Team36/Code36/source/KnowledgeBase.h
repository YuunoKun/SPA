#pragma once

#include <string>
#include "Common.h"
#include "Token.h"
#include "PKB.h"

class KnowledgeBase {
public:
	virtual ~KnowledgeBase() {}

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

	virtual void generateParentT() = 0;
	virtual void generateFollowsT() = 0;
};
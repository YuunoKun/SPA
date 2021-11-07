#pragma once

#include <vector>

#include "Common.h"
#include "CFG.h"

class PKBSourceInterface {
public:
	virtual void addConstant(constant constant) = 0;
	virtual void addProcedure(proc_name proc_name) = 0;
	virtual void addVariable(var_name var_name) = 0;
	virtual void addStmt(StmtType stmt_type) = 0;
	virtual void addExprTree(stmt_index stmt_index, expr expr) = 0;

	virtual void addParent(stmt_index parent, stmt_index child) = 0;
	virtual void addParentT(stmt_index parent, stmt_index child) = 0;
	virtual void addFollows(stmt_index first, stmt_index second) = 0;
	virtual void addFollowsT(stmt_index first, stmt_index second) = 0;
	virtual void addUsesS(stmt_index user, var_name used) = 0;
	virtual void addModifiesS(stmt_index modifier, var_name modified) = 0;
	virtual void addModifiesP(proc_name proc, var_name modified) = 0;
	virtual void addUsesP(proc_name proc, var_name used) = 0;

	virtual void addNext(prog_line prog_line1, prog_line prog_line2) = 0;
	virtual void addCallsP(proc_name caller_proc_name, proc_name callee_proc_name) = 0;
	virtual void addCallsPT(proc_name caller_proc_name, proc_name callee_proc_name) = 0;
	virtual void addCallsS(stmt_index caller_stmt_index, proc_name callee_proc_name) = 0;
	virtual void addIf(stmt_index if_stmt_index, var_name control_var) = 0;
	virtual void addWhile(stmt_index while_stmt_index, var_name control_var) = 0;
	virtual void addProcContains(proc_name proc, stmt_index index) = 0;

	virtual void addCFGsToDestroy(std::vector<CFG*>) = 0;
	virtual void addCFGBip(CFG*) = 0;
};
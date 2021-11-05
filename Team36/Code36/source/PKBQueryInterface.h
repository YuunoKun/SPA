#pragma once

#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "Common.h"
#include "RelationTable.h"
#include "MonotypeRelationTable.h"
#include "CFG.h"

class PKBQueryInterface {
public:
	virtual const std::vector<proc_name>& getProcedures() = 0;
	virtual const std::vector<var_name>& getVariables() = 0;
	virtual const std::vector<StmtInfo>& getStmts() = 0;
	virtual const std::unordered_map<stmt_index, expr>& getExpr() = 0;
	virtual const StmtInfo getStmt(stmt_index stmt_index) = 0;
	virtual const var_name getAssignment(stmt_index stmt_index) = 0;
	virtual expr getExpression(stmt_index stmt_index) = 0;
	virtual const std::vector<constant> getConstants() = 0;

	virtual const RelationTable<stmt_index, var_name>& getAssigns() = 0;
	virtual const MonotypeRelationTable<StmtInfo>& getFollows() = 0;
	virtual const MonotypeRelationTable<StmtInfo>& getParent() = 0;
	virtual const MonotypeRelationTable<StmtInfo>& getFollowsT() = 0;
	virtual const MonotypeRelationTable<StmtInfo>& getParentT() = 0;
	virtual const RelationTable<StmtInfo, var_name>& getUsesS() = 0;
	virtual const RelationTable<StmtInfo, var_name>& getModifiesS() = 0;
	virtual const RelationTable<proc_name, var_name>& getUsesP() = 0;
	virtual const RelationTable<proc_name, var_name>& getModifiesP() = 0;
	virtual const MonotypeRelationTable<proc_name>& getCallsP() = 0;
	virtual const MonotypeRelationTable<proc_name>& getCallsPT() = 0;
	virtual const RelationTable<stmt_index, proc_name>& getCallsS() = 0;
	virtual const RelationTable<stmt_index, var_name>& getRead() = 0;
	virtual const RelationTable<stmt_index, var_name>& getPrint() = 0;
	virtual const RelationTable<stmt_index, var_name>& getIf() = 0;
	virtual const RelationTable<stmt_index, var_name>& getWhile() = 0;
	virtual const MonotypeRelationTable<StmtInfo>& getNext() = 0;
	virtual const RelationTable<proc_name, stmt_index>& getProcContains() = 0;
	virtual std::vector<CFG*> getCFGBips() = 0;

	virtual void resetCache() = 0;
	virtual void resetEntities() = 0;

};
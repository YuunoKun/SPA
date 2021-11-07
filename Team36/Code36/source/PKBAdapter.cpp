#include "PKBAdapter.h"
#include "PKB.h"
#include "Utility.h"
#include "Common.h"
#include "RelationTable.h"
#include "RelationTable.cpp"

PKBAdapter::PKBAdapter()
	: pkb_instance(PKB::getInstance()) {
}

PKBAdapter::PKBAdapter(PKBQueryInterface& pkb)
	: pkb_instance(pkb) {
}

StmtInfo PKBAdapter::getStmt(stmt_index index) {
	return pkb_instance.getStmt(index);
}

std::vector<var_name> PKBAdapter::getVariables() {
	return pkb_instance.getVariables();
}

std::vector<StmtInfo> PKBAdapter::getStmts() {
	return pkb_instance.getStmts();
}

std::vector<constant> PKBAdapter::getConstants() {
	return pkb_instance.getConstants();
}

std::vector<proc_name> PKBAdapter::getProcedures() {
	return pkb_instance.getProcedures();
}

bool PKBAdapter::isVariable(var_name var) {
	auto a = pkb_instance.getVariablesSet();
	return a.count(var) != 0;
}

bool PKBAdapter::isStmt(stmt_index index) {
	return index > 0 && index <= getStmts().size();
}

bool PKBAdapter::isConstant(constant c) {
	auto a = pkb_instance.getConstantsSet();
	return a.count(c) != 0;
}

bool PKBAdapter::isProcedure(proc_name proc) {
	auto a = pkb_instance.getProceduresSet();
	return a.count(proc) != 0;
}

bool PKBAdapter::isAssign(stmt_index index) {
	return isStmt(index) && getStmt(index).stmt_type == StmtType::STMT_ASSIGN;
}

bool PKBAdapter::isPrint(stmt_index index) {
	return isStmt(index) && getStmt(index).stmt_type == StmtType::STMT_PRINT;
}

bool PKBAdapter::isCall(stmt_index index) {
	return isStmt(index) && getStmt(index).stmt_type == StmtType::STMT_CALL;
}

bool PKBAdapter::isRead(stmt_index index) {
	return isStmt(index) && getStmt(index).stmt_type == StmtType::STMT_READ;
}

bool PKBAdapter::isWhile(stmt_index index) {
	return isStmt(index) && getStmt(index).stmt_type == StmtType::STMT_WHILE;
}

bool PKBAdapter::isIf(stmt_index index) {
	return isStmt(index) && getStmt(index).stmt_type == StmtType::STMT_IF;
}

std::vector<pattern_info> PKBAdapter::getAssignInfo() {
	auto a = pkb_instance.getAssigns();
	return a.getPairs();
}

std::vector<pattern_info> PKBAdapter::getAssignInfo(expr matchExpression, bool isWild) {
	std::vector<pattern_info> temp = getAssignInfo();
	std::vector<pattern_info> a;
	for (auto s : temp) {
		expr expression = pkb_instance.getExpression(s.first);
		if (isWild && expression.contains(&matchExpression)) {
			a.push_back(s);
		} else if (expression.equals(&matchExpression)) {
			a.push_back(s);
		}
	}
	return a;
}

std::vector<stmt_index> PKBAdapter::getAssignInfoFiltered() {
	auto a = pkb_instance.getAssigns();
	return a.getKeys();
}

std::vector<stmt_index> PKBAdapter::getAssignInfoFiltered(expr matchExpression, bool isWild) {
	return getAssignInfoFiltered(getAssignInfoFiltered(), matchExpression, isWild);
}

std::vector<stmt_index> PKBAdapter::getAssignInfoFiltered(var_name var) {
	auto a = pkb_instance.getAssigns();
	return a.getKeys(var);
}

std::vector<stmt_index> PKBAdapter::getAssignInfoFiltered(var_name var, expr matchExpression, bool isWild) {
	return getAssignInfoFiltered(getAssignInfoFiltered(var), matchExpression, isWild);
}

std::vector<stmt_index> PKBAdapter::getAssignInfoFiltered(std::vector<stmt_index>& temp, expr matchExpression, bool isWild) {
	std::vector<stmt_index> a;
	for (auto s : temp) {
		expr expression = pkb_instance.getExpression(s);
		if (isWild && expression.contains(&matchExpression)) {
			a.push_back(s);
		} else if (expression.equals(&matchExpression)) {
			a.push_back(s);
		}
	}
	return a;
}

bool PKBAdapter::isFollowEmpty() {
	auto a = pkb_instance.getFollows();
	return a.isEmpty();
}

bool PKBAdapter::isFollow(stmt_index index1, stmt_index index2) {
	auto a = pkb_instance.getFollows();
	//// Need better method to find stmt_info
	StmtInfo s1 = getStmt(index1);
	StmtInfo s2 = getStmt(index2);
	return a.containsPair(s1, s2);
}

bool PKBAdapter::isFollowed(stmt_index index) {
	auto a = pkb_instance.getFollows();
	StmtInfo s1 = getStmt(index);
	return a.containsKey(s1);
}

bool PKBAdapter::isFollowing(stmt_index index) {
	auto a = pkb_instance.getFollows();
	StmtInfo s1 = getStmt(index);
	return a.containsValue(s1);
}

std::vector<StmtInfo> PKBAdapter::getFollowed() {
	auto a = pkb_instance.getFollows();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getFollowing() {
	auto a = pkb_instance.getFollows();
	return a.getValues();
}

std::vector<StmtInfo> PKBAdapter::getFollowed(stmt_index index) {
	auto a = pkb_instance.getFollows();
	StmtInfo s1 = getStmt(index);
	return a.getKeys(s1);
}

std::vector<StmtInfo> PKBAdapter::getFollowing(stmt_index index) {
	auto a = pkb_instance.getFollows();
	StmtInfo s1 = getStmt(index);
	return a.getValues(s1);
}

std::vector<std::pair<StmtInfo, StmtInfo>> PKBAdapter::getFollows() {
	auto a = pkb_instance.getFollows();
	return a.getPairs();
}

bool PKBAdapter::isFollowTEmpty() {
	auto a = pkb_instance.getFollowsT();
	return a.isEmpty();
}

bool PKBAdapter::isFollowT(stmt_index index1, stmt_index index2) {
	auto a = pkb_instance.getFollowsT();
	StmtInfo s1 = getStmt(index1);
	StmtInfo s2 = getStmt(index2);
	return a.containsPair(s1, s2);
}

bool PKBAdapter::isFollowedT(stmt_index index) {
	auto a = pkb_instance.getFollowsT();
	StmtInfo s1 = getStmt(index);
	return a.containsKey(s1);
}

bool PKBAdapter::isFollowingT(stmt_index index) {
	auto a = pkb_instance.getFollowsT();
	StmtInfo s1 = getStmt(index);
	return a.containsValue(s1);
}

std::vector<StmtInfo> PKBAdapter::getFollowedT() {
	auto a = pkb_instance.getFollowsT();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getFollowingT() {
	auto a = pkb_instance.getFollowsT();
	return a.getValues();
}

std::vector<StmtInfo> PKBAdapter::getFollowedT(stmt_index index) {
	auto a = pkb_instance.getFollowsT();
	StmtInfo s1 = getStmt(index);
	return a.getKeys(s1);
}

std::vector<StmtInfo> PKBAdapter::getFollowingT(stmt_index index) {
	auto a = pkb_instance.getFollowsT();
	StmtInfo s1 = getStmt(index);
	return a.getValues(s1);
}

std::vector<std::pair<StmtInfo, StmtInfo>> PKBAdapter::getFollowsT() {
	auto a = pkb_instance.getFollowsT();
	return a.getPairs();
}

bool PKBAdapter::isParentEmpty() {
	auto a = pkb_instance.getParent();
	return a.isEmpty();
}

bool PKBAdapter::isParent(stmt_index index1, stmt_index index2) {
	auto a = pkb_instance.getParent();
	StmtInfo s1 = getStmt(index1);
	StmtInfo s2 = getStmt(index2);
	return a.containsPair(s1, s2);
}

bool PKBAdapter::isChild(stmt_index index) {
	auto a = pkb_instance.getParent();
	StmtInfo s1 = getStmt(index);
	return a.containsValue(s1);
}

bool PKBAdapter::isParent(stmt_index index) {
	auto a = pkb_instance.getParent();
	StmtInfo s1 = getStmt(index);
	return a.containsKey(s1);
}

std::vector<StmtInfo> PKBAdapter::getChild() {
	auto a = pkb_instance.getParent();
	return a.getValues();
}

std::vector<StmtInfo> PKBAdapter::getParent() {
	auto a = pkb_instance.getParent();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getChild(stmt_index index) {
	auto a = pkb_instance.getParent();
	StmtInfo s1 = getStmt(index);
	return a.getValues(s1);
}

std::vector<StmtInfo> PKBAdapter::getParent(stmt_index index) {
	auto a = pkb_instance.getParent();
	StmtInfo s1 = getStmt(index);
	return a.getKeys(s1);
}

std::vector<std::pair<StmtInfo, StmtInfo>> PKBAdapter::getAllParentRelation() {
	auto a = pkb_instance.getParent();
	return a.getPairs();
}

bool PKBAdapter::isParentTEmpty() {
	auto a = pkb_instance.getParentT();
	return a.isEmpty();
}

bool PKBAdapter::isParentT(stmt_index index1, stmt_index index2) {
	auto a = pkb_instance.getParentT();
	StmtInfo s1 = getStmt(index1);
	StmtInfo s2 = getStmt(index2);
	return a.containsPair(s1, s2);
}

bool PKBAdapter::isChildT(stmt_index index) {
	auto a = pkb_instance.getParentT();
	StmtInfo s1 = getStmt(index);
	return a.containsValue(s1);
}

bool PKBAdapter::isParentT(stmt_index index) {
	auto a = pkb_instance.getParentT();
	StmtInfo s1 = getStmt(index);
	return a.containsKey(s1);
}

std::vector<StmtInfo> PKBAdapter::getChildT() {
	auto a = pkb_instance.getParentT();
	return a.getValues();
}

std::vector<StmtInfo> PKBAdapter::getParentT() {
	auto a = pkb_instance.getParentT();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getChildT(stmt_index index) {
	auto a = pkb_instance.getParentT();
	StmtInfo s1 = getStmt(index);
	return a.getValues(s1);
}

std::vector<StmtInfo> PKBAdapter::getParentT(stmt_index index) {
	auto a = pkb_instance.getParentT();
	StmtInfo s1 = getStmt(index);
	return a.getKeys(s1);
}

std::vector<std::pair<StmtInfo, StmtInfo>> PKBAdapter::getAllParentTRelation() {
	auto a = pkb_instance.getParentT();
	return a.getPairs();
}

bool PKBAdapter::isModifiesP(proc_name proc, var_name var) {
	auto a = pkb_instance.getModifiesP();
	return a.containsPair(proc, var);
}

bool PKBAdapter::isModifiesP(proc_name proc) {
	auto a = pkb_instance.getModifiesP();
	return a.containsKey(proc);
}

std::vector<std::pair<proc_name, var_name>> PKBAdapter::getModifiesPRelation() {
	auto a = pkb_instance.getModifiesP();
	return a.getPairs();
}

std::vector<proc_name> PKBAdapter::getModifiesP() {
	auto a = pkb_instance.getModifiesP();
	return a.getKeys();
}

std::vector<proc_name> PKBAdapter::getModifiesP(var_name var) {
	auto a = pkb_instance.getModifiesP();
	return a.getKeys(var);
}

std::vector<var_name> PKBAdapter::getModifiedP(proc_name proc) {
	auto a = pkb_instance.getModifiesP();
	return a.getValues(proc);
}

bool PKBAdapter::isModifiesS(stmt_index index, var_name var) {
	auto a = pkb_instance.getModifiesS();
	StmtInfo s1 = getStmt(index);
	return a.containsPair(s1, var);
}

bool PKBAdapter::isModifiesS(stmt_index index) {
	auto a = pkb_instance.getModifiesS();
	StmtInfo s1 = getStmt(index);
	return a.containsKey(s1);
}

std::vector<std::pair<StmtInfo, var_name>> PKBAdapter::getModifiesSRelation() {
	auto a = pkb_instance.getModifiesS();
	return a.getPairs();
}

std::vector<StmtInfo> PKBAdapter::getModifiesS() {
	auto a = pkb_instance.getModifiesS();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getModifiesS(var_name var) {
	auto a = pkb_instance.getModifiesS();
	return a.getKeys(var);
}

std::vector<var_name> PKBAdapter::getModifiedS(stmt_index index) {
	auto a = pkb_instance.getModifiesS();
	StmtInfo s1 = getStmt(index);
	return a.getValues(s1);
}

bool PKBAdapter::isUsesP(proc_name proc, var_name var) {
	auto a = pkb_instance.getUsesP();
	return a.containsPair(proc, var);
}

bool PKBAdapter::isUsesP(proc_name proc) {
	auto a = pkb_instance.getUsesP();
	return a.containsKey(proc);
}

std::vector<std::pair<proc_name, var_name>> PKBAdapter::getUsesPRelation() {
	auto a = pkb_instance.getUsesP();
	return a.getPairs();
}

std::vector<proc_name> PKBAdapter::getUsesP() {
	auto a = pkb_instance.getUsesP();
	return a.getKeys();
}

std::vector<proc_name> PKBAdapter::getUsesP(var_name var) {
	auto a = pkb_instance.getUsesP();
	return a.getKeys(var);
}

std::vector<var_name> PKBAdapter::getUsedP(proc_name proc) {
	auto a = pkb_instance.getUsesP();
	return a.getValues(proc);
}

bool PKBAdapter::isUsesS(stmt_index index, var_name var) {
	auto a = pkb_instance.getUsesS();
	StmtInfo s1 = getStmt(index);
	return a.containsPair(s1, var);
}

bool PKBAdapter::isUsesS(stmt_index index) {
	auto a = pkb_instance.getUsesS();
	StmtInfo s1 = getStmt(index);
	return a.containsKey(s1);
}

std::vector<std::pair<StmtInfo, var_name>> PKBAdapter::getUsesSRelation() {
	auto a = pkb_instance.getUsesS();
	return a.getPairs();
}

std::vector<StmtInfo> PKBAdapter::getUsesS() {
	auto a = pkb_instance.getUsesS();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getUsesS(var_name var) {
	auto a = pkb_instance.getUsesS();
	return a.getKeys(var);
}

std::vector<var_name> PKBAdapter::getUsedS(stmt_index index) {
	auto a = pkb_instance.getUsesS();
	StmtInfo s1 = getStmt(index);
	return a.getValues(s1);
}

bool PKBAdapter::isCallsPEmpty() {
	auto a = pkb_instance.getCallsP();
	return a.isEmpty();
}

bool PKBAdapter::isCallsP(proc_name proc1, proc_name proc2) {
	auto a = pkb_instance.getCallsP();
	return a.containsPair(proc1, proc2);
}

bool PKBAdapter::isCallerP(proc_name proc) {
	auto a = pkb_instance.getCallsP();
	return a.containsKey(proc);
}

bool PKBAdapter::isCalleeP(proc_name proc) {
	auto a = pkb_instance.getCallsP();
	return a.containsValue(proc);
}

std::vector<proc_name> PKBAdapter::getCallerP() {
	auto a = pkb_instance.getCallsP();
	return a.getKeys();
}

std::vector<proc_name> PKBAdapter::getCalleeP() {
	auto a = pkb_instance.getCallsP();
	return a.getValues();
}

std::vector<proc_name> PKBAdapter::getCallerP(proc_name proc) {
	auto a = pkb_instance.getCallsP();
	return a.getKeys(proc);
}

std::vector<proc_name> PKBAdapter::getCalleeP(proc_name proc) {
	auto a = pkb_instance.getCallsP();
	return a.getValues(proc);
}

std::vector<std::pair<proc_name, proc_name>> PKBAdapter::getCallsPRelation() {
	auto a = pkb_instance.getCallsP();
	return a.getPairs();
}

bool PKBAdapter::isCallsPTEmpty() {
	auto a = pkb_instance.getCallsPT();
	return a.isEmpty();
}

bool PKBAdapter::isCallsPT(proc_name proc1, proc_name proc2) {
	auto a = pkb_instance.getCallsPT();
	return a.containsPair(proc1, proc2);
}

bool PKBAdapter::isCallerPT(proc_name proc) {
	auto a = pkb_instance.getCallsPT();
	return a.containsKey(proc);
}

bool PKBAdapter::isCalleePT(proc_name proc) {
	auto a = pkb_instance.getCallsPT();
	return a.containsValue(proc);
}

std::vector<proc_name> PKBAdapter::getCallerPT() {
	auto a = pkb_instance.getCallsPT();
	return a.getKeys();
}

std::vector<proc_name> PKBAdapter::getCalleePT() {
	auto a = pkb_instance.getCallsPT();
	return a.getValues();
}

std::vector<proc_name> PKBAdapter::getCallerPT(proc_name proc) {
	auto a = pkb_instance.getCallsPT();
	return a.getKeys(proc);
}

std::vector<proc_name> PKBAdapter::getCalleePT(proc_name proc) {
	auto a = pkb_instance.getCallsPT();
	return a.getValues(proc);
}

std::vector<std::pair<proc_name, proc_name>> PKBAdapter::getCallsPTRelation() {
	auto a = pkb_instance.getCallsPT();
	return a.getPairs();
}

bool PKBAdapter::isNextEmpty() {
	auto a = pkb_instance.getNext();
	return a.isEmpty();
}

bool PKBAdapter::isNext(prog_line index1, prog_line index2) {
	auto a = pkb_instance.getNext();
	StmtInfo s1 = getStmt(index1);
	StmtInfo s2 = getStmt(index2);
	return a.containsPair(s1, s2);
}

bool PKBAdapter::isPrevious(prog_line index) {
	auto a = pkb_instance.getNext();
	StmtInfo s1 = getStmt(index);
	return a.containsKey(s1);
}

bool PKBAdapter::isNext(prog_line index) {
	auto a = pkb_instance.getNext();
	StmtInfo s1 = getStmt(index);
	return a.containsValue(s1);
}

std::vector<std::pair<StmtInfo, StmtInfo>> PKBAdapter::getAllNextRelation() {
	auto a = pkb_instance.getNext();
	return a.getPairs();
}

std::vector<StmtInfo> PKBAdapter::getPrevious() {
	auto a = pkb_instance.getNext();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getNext() {
	auto a = pkb_instance.getNext();
	return a.getValues();
}

std::vector<StmtInfo> PKBAdapter::getPrevious(prog_line index) {
	auto a = pkb_instance.getNext();
	StmtInfo s1 = getStmt(index);
	return a.getKeys(s1);
}

std::vector<StmtInfo> PKBAdapter::getNext(prog_line index) {
	auto a = pkb_instance.getNext();
	StmtInfo s1 = getStmt(index);
	return a.getValues(s1);
}

std::vector<std::pair<stmt_index, var_name>> PKBAdapter::getAllIfUses() {
	auto a = pkb_instance.getIf();
	return a.getPairs();
}

std::vector<stmt_index> PKBAdapter::getIfUses() {
	auto a = pkb_instance.getIf();
	return a.getKeys();
}

std::vector<stmt_index> PKBAdapter::getIfUses(var_name var) {
	auto a = pkb_instance.getIf();
	return a.getKeys(var);
}

std::vector<std::pair<stmt_index, var_name>> PKBAdapter::getAllWhileUses() {
	auto a = pkb_instance.getWhile();
	return a.getPairs();
}

std::vector<stmt_index> PKBAdapter::getWhileUses() {
	auto a = pkb_instance.getWhile();
	return a.getKeys();
}

std::vector<stmt_index> PKBAdapter::getWhileUses(var_name var) {
	auto a = pkb_instance.getWhile();
	return a.getKeys(var);
}

std::vector<std::pair<stmt_index, var_name>> PKBAdapter::getAllPrintVars() {
	auto a = pkb_instance.getPrint();
	return a.getPairs();
}

std::vector<var_name> PKBAdapter::getPrintVar(stmt_index index) {
	auto a = pkb_instance.getPrint();
	return a.getValues(index);
}

std::vector<var_name> PKBAdapter::getPrintVar() {
	auto a = pkb_instance.getPrint();
	return a.getValues();
}

bool PKBAdapter::isPrintVar(var_name v) {
	auto a = pkb_instance.getPrint();
	return a.containsValue(v);
}

std::vector<stmt_index> PKBAdapter::getPrint(var_name var) {
	auto a = pkb_instance.getPrint();
	return a.getKeys(var);
}

std::vector<std::pair<stmt_index, var_name>> PKBAdapter::getAllReadVars() {
	auto a = pkb_instance.getRead();
	return a.getPairs();
}

std::vector<var_name> PKBAdapter::getReadVar(stmt_index index) {
	auto a = pkb_instance.getRead();
	return a.getValues(index);
}

std::vector<var_name> PKBAdapter::getReadVar() {
	auto a = pkb_instance.getRead();
	return a.getValues();
}

bool PKBAdapter::isReadVar(var_name v) {
	auto a = pkb_instance.getRead();
	return a.containsValue(v);
}

std::vector<stmt_index> PKBAdapter::getRead(var_name var) {
	auto a = pkb_instance.getRead();
	return a.getKeys(var);
}

std::vector<std::pair<stmt_index, proc_name>> PKBAdapter::getAllCallS() {
	auto a = pkb_instance.getCallsS();
	return a.getPairs();
}

std::vector<proc_name> PKBAdapter::getCalledS(stmt_index index) {
	auto a = pkb_instance.getCallsS();
	return a.getValues(index);
}

std::vector<proc_name> PKBAdapter::getCalledS() {
	auto a = pkb_instance.getCallsS();
	return a.getValues();
}

bool PKBAdapter::isCalledS(proc_name p) {
	auto a = pkb_instance.getCallsS();
	return a.containsValue(p);
}

std::vector<stmt_index> PKBAdapter::getCalleeS(proc_name proc) {
	auto a = pkb_instance.getCallsS();
	return a.getKeys(proc);
}

RelationPreprocessorManager& PKBAdapter::getRelationManager() {
	static RelationPreprocessorManager relation_manager;
	return relation_manager;
}
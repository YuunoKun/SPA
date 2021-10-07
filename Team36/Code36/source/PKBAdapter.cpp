#include "PKBAdapter.h"
#include "PKB.h"
#include "Utility.h"
#include "Common.h"
#include "Stmt.h"
#include "RelationTable.h"
#include "RelationTable.cpp"

PKBAdapter::PKBAdapter() {
}

StmtInfo PKBAdapter::getStmt(stmt_index index)
{
	return PKB::getInstance().PKB::getStmt(index);
}

std::vector<var_name> PKBAdapter::getVariables() {
	return PKB::getInstance().PKB::getVariables();
}

std::vector<StmtInfo> PKBAdapter::getStmts() {
	return PKB::getInstance().PKB::getStmts();
}

std::vector<constant> PKBAdapter::getConstants() {
	return PKB::getInstance().getConstants();
}

std::vector<proc_name> PKBAdapter::getProcedures() {
	return PKB::getInstance().PKB::getProcedures();
}

std::vector<StmtInfo> PKBAdapter::getAssigns() {
	std::vector<StmtInfo> v = getStmts();
	return Utility::filterResult(ASSIGN, v);
}

std::vector<StmtInfo> PKBAdapter::getPrints() {
	std::vector<StmtInfo> v = getStmts();
	return Utility::filterResult(PRINT, v);
}

std::vector<StmtInfo> PKBAdapter::getCalls() {
	std::vector<StmtInfo> v = getStmts();
	return Utility::filterResult(CALL, v);
}

std::vector<StmtInfo> PKBAdapter::getReads() {
	std::vector<StmtInfo> v = getStmts();
	return Utility::filterResult(READ, v);
}

std::vector<StmtInfo> PKBAdapter::getWhiles() {
	std::vector<StmtInfo> v = getStmts();
	return Utility::filterResult(WHILE, v);
}

std::vector<StmtInfo> PKBAdapter::getIfs() {
	std::vector<StmtInfo> v = getStmts();
	return Utility::filterResult(IF, v);
}

std::vector<assign_info> PKBAdapter::getAssignInfo() {
	std::vector<StmtInfo> s = getAssigns();
	std::vector<assign_info> a;
	for (auto s : s) {
		assign_info i = std::make_pair(s, PKB::getInstance().PKB::getAssignment(s.stmt_index));
		a.push_back(i);
	}
	return a;
}

std::vector<assign_info> PKBAdapter::getAssignInfo(std::string matchExpression, bool isWild) {
	std::vector<StmtInfo> s = getAssigns();
	std::vector<assign_info> a;
	for (auto s : s) {
		std::string expression = PKB::getInstance().PKB::getExpression(s.stmt_index);
		if (isWild && Utility::patternContain(expression, matchExpression)) {
			assign_info i = std::make_pair(s, PKB::getInstance().PKB::getAssignment(s.stmt_index));
			a.push_back(i);
		}
		else if (Utility::patternMatch(expression, matchExpression)) {
			assign_info i = std::make_pair(s, PKB::getInstance().PKB::getAssignment(s.stmt_index));
			a.push_back(i);
		}
	}
	return a;
}

bool PKBAdapter::isFollowEmpty() {
	auto a = PKB::getInstance().PKB::getFollows();
	return a.isEmpty();
}

bool PKBAdapter::isFollow(stmt_index index1, stmt_index index2) {
	auto a = PKB::getInstance().PKB::getFollows();
	//// Need better method to find stmt_info
	StmtInfo s1 = getStmt(index1);
	StmtInfo s2 = getStmt(index2);
	return a.containsPair(s1, s2);
}

bool PKBAdapter::isFollowed(stmt_index index) {
	auto a = PKB::getInstance().PKB::getFollows();
	StmtInfo s1 = getStmt(index);
	return a.containsKey(s1);
}

bool PKBAdapter::isFollowing(stmt_index index) {
	auto a = PKB::getInstance().PKB::getFollows();
	StmtInfo s1 = getStmt(index);
	return a.containsValue(s1);
}

std::vector<StmtInfo> PKBAdapter::getFollowed() {
	auto a = PKB::getInstance().PKB::getFollows();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getFollowing() {
	auto a = PKB::getInstance().PKB::getFollows();
	return a.getValues();
}

std::vector<StmtInfo> PKBAdapter::getFollowed(stmt_index index) {
	auto a = PKB::getInstance().PKB::getFollows();
	StmtInfo s1 = getStmt(index);
	return a.getKeys(s1);
}

std::vector<StmtInfo> PKBAdapter::getFollowing(stmt_index index) {
	auto a = PKB::getInstance().PKB::getFollows();
	StmtInfo s1 = getStmt(index);
	return a.getValues(s1);
}

std::vector<std::pair<StmtInfo, StmtInfo>> PKBAdapter::getFollows() {
	auto a = PKB::getInstance().PKB::getFollows();
	return a.getPairs();
}

bool PKBAdapter::isFollowTEmpty() {
	auto a = PKB::getInstance().PKB::getFollowsT();
	return a.isEmpty();
}

bool PKBAdapter::isFollowT(stmt_index index1, stmt_index index2) {
	auto a = PKB::getInstance().PKB::getFollowsT();
	//// Need better method to find stmt_info
	StmtInfo s1 = getStmt(index1);
	StmtInfo s2 = getStmt(index2);
	return a.containsPair(s1, s2);
}

bool PKBAdapter::isFollowedT(stmt_index index) {
	auto a = PKB::getInstance().PKB::getFollowsT();
	StmtInfo s1 = getStmt(index);
	return a.containsKey(s1);
}

bool PKBAdapter::isFollowingT(stmt_index index) {
	auto a = PKB::getInstance().PKB::getFollowsT();
	StmtInfo s1 = getStmt(index);
	return a.containsValue(s1);
}

std::vector<StmtInfo> PKBAdapter::getFollowedT() {
	auto a = PKB::getInstance().PKB::getFollowsT();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getFollowingT() {
	auto a = PKB::getInstance().PKB::getFollowsT();
	return a.getValues();
}

std::vector<StmtInfo> PKBAdapter::getFollowedT(stmt_index index) {
	auto a = PKB::getInstance().PKB::getFollowsT();
	StmtInfo s1 = getStmt(index);
	return a.getKeys(s1);
}

std::vector<StmtInfo> PKBAdapter::getFollowingT(stmt_index index) {
	auto a = PKB::getInstance().PKB::getFollowsT();
	StmtInfo s1 = getStmt(index);
	return a.getValues(s1);
}

std::vector<std::pair<StmtInfo, StmtInfo>> PKBAdapter::getFollowsT() {
	auto a = PKB::getInstance().PKB::getFollowsT();
	return a.getPairs();
}

bool PKBAdapter::isParentEmpty() {
	auto a = PKB::getInstance().PKB::getParent();
	return a.isEmpty();
}

bool PKBAdapter::isParent(stmt_index index1, stmt_index index2) {
	auto a = PKB::getInstance().PKB::getParent();
	//// Need better method to find stmt_info
	StmtInfo s1 = getStmt(index1);
	StmtInfo s2 = getStmt(index2);
	return a.containsPair(s1, s2);
}

bool PKBAdapter::isChild(stmt_index index) {
	auto a = PKB::getInstance().PKB::getParent();
	StmtInfo s1 = getStmt(index);
	return a.containsValue(s1);
}

bool PKBAdapter::isParent(stmt_index index) {
	auto a = PKB::getInstance().PKB::getParent();
	StmtInfo s1 = getStmt(index);
	return a.containsKey(s1);
}

std::vector<StmtInfo> PKBAdapter::getChild() {
	auto a = PKB::getInstance().PKB::getParent();
	return a.getValues();
}

std::vector<StmtInfo> PKBAdapter::getParent() {
	auto a = PKB::getInstance().PKB::getParent();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getChild(stmt_index index) {
	auto a = PKB::getInstance().PKB::getParent();
	StmtInfo s1 = getStmt(index);
	return a.getValues(s1);
}

std::vector<StmtInfo> PKBAdapter::getParent(stmt_index index) {
	auto a = PKB::getInstance().PKB::getParent();
	StmtInfo s1 = getStmt(index);
	return a.getKeys(s1);
}

std::vector<std::pair<StmtInfo, StmtInfo>> PKBAdapter::getAllParentRelation() {
	auto a = PKB::getInstance().PKB::getParent();
	return a.getPairs();
}

bool PKBAdapter::isParentTEmpty() {
	auto a = PKB::getInstance().PKB::getParentT();
	return a.isEmpty();
}

bool PKBAdapter::isParentT(stmt_index index1, stmt_index index2) {
	auto a = PKB::getInstance().PKB::getParentT();
	StmtInfo s1 = getStmt(index1);
	StmtInfo s2 = getStmt(index2);
	return a.containsPair(s1, s2);
}

bool PKBAdapter::isChildT(stmt_index index) {
	auto a = PKB::getInstance().PKB::getParentT();
	StmtInfo s1 = getStmt(index);
	return a.containsValue(s1);
}

bool PKBAdapter::isParentT(stmt_index index) {
	auto a = PKB::getInstance().PKB::getParentT();
	StmtInfo s1 = getStmt(index);
	return a.containsKey(s1);
}

std::vector<StmtInfo> PKBAdapter::getChildT() {
	auto a = PKB::getInstance().PKB::getParentT();
	return a.getValues();
}

std::vector<StmtInfo> PKBAdapter::getParentT() {
	auto a = PKB::getInstance().PKB::getParentT();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getChildT(stmt_index index) {
	auto a = PKB::getInstance().PKB::getParentT();
	StmtInfo s1 = getStmt(index);
	return a.getValues(s1);
}

std::vector<StmtInfo> PKBAdapter::getParentT(stmt_index index) {
	auto a = PKB::getInstance().PKB::getParentT();
	StmtInfo s1 = getStmt(index);
	return a.getKeys(s1);
}

std::vector<std::pair<StmtInfo, StmtInfo>> PKBAdapter::getAllParentTRelation() {
	auto a = PKB::getInstance().PKB::getParentT();
	return a.getPairs();
}

bool PKBAdapter::isModifiesP(proc_name proc, var_name var) {
	auto a = PKB::getInstance().PKB::getModifiesP();
	return a.containsPair(proc, var);
}

bool PKBAdapter::isModifiesP(proc_name proc) {
	auto a = PKB::getInstance().PKB::getModifiesP();
	return a.containsKey(proc);
}

std::vector<std::pair<proc_name, var_name>> PKBAdapter::getModifiesPRelation() {
	auto a = PKB::getInstance().PKB::getModifiesP();
	return a.getPairs();
}

std::vector<proc_name> PKBAdapter::getModifiesP() {
	auto a = PKB::getInstance().PKB::getModifiesP();
	return a.getKeys();
}

std::vector<proc_name> PKBAdapter::getModifiesP(var_name var) {
	auto a = PKB::getInstance().PKB::getModifiesP();
	return a.getKeys(var);
}

std::vector<var_name> PKBAdapter::getModifiedP(proc_name proc) {
	auto a = PKB::getInstance().PKB::getModifiesP();
	return a.getValues(proc);
}

bool PKBAdapter::isModifiesS(stmt_index index, var_name var) {
	auto a = PKB::getInstance().PKB::getModifiesS();
	StmtInfo s1 = getStmt(index);
	return a.containsPair(s1, var);
}

bool PKBAdapter::isModifiesS(stmt_index index) {
	auto a = PKB::getInstance().PKB::getModifiesS();
	StmtInfo s1 = getStmt(index);
	return a.containsKey(s1);
}

std::vector<std::pair<StmtInfo, var_name>> PKBAdapter::getModifiesSRelation() {
	auto a = PKB::getInstance().PKB::getModifiesS();
	return a.getPairs();
}

std::vector<StmtInfo> PKBAdapter::getModifiesS() {
	auto a = PKB::getInstance().PKB::getModifiesS();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getModifiesS(var_name var) {
	auto a = PKB::getInstance().PKB::getModifiesS();
	return a.getKeys(var);
}

std::vector<var_name> PKBAdapter::getModifiedS(stmt_index index) {
	auto a = PKB::getInstance().PKB::getModifiesS();
	StmtInfo s1 = getStmt(index);
	return a.getValues(s1);
}

bool PKBAdapter::isUsesP(proc_name proc, var_name var) {
	auto a = PKB::getInstance().PKB::getUsesP();
	return a.containsPair(proc, var);
}

bool PKBAdapter::isUsesP(proc_name proc) {
	auto a = PKB::getInstance().PKB::getUsesP();
	return a.containsKey(proc);
}

std::vector<std::pair<proc_name, var_name>> PKBAdapter::getUsesPRelation() {
	auto a = PKB::getInstance().PKB::getUsesP();
	return a.getPairs();
}

std::vector<proc_name> PKBAdapter::getUsesP() {
	auto a = PKB::getInstance().PKB::getUsesP();
	return a.getKeys();
}

std::vector<proc_name> PKBAdapter::getUsesP(var_name var) {
	auto a = PKB::getInstance().PKB::getUsesP();
	return a.getKeys(var);
}

std::vector<var_name> PKBAdapter::getUsedP(proc_name proc) {
	auto a = PKB::getInstance().PKB::getUsesP();
	return a.getValues(proc);
}

bool PKBAdapter::isUsesS(stmt_index index, var_name var) {
	auto a = PKB::getInstance().PKB::getUsesS();
	StmtInfo s1 = getStmt(index);
	return a.containsPair(s1, var);
}

bool PKBAdapter::isUsesS(stmt_index index) {
	auto a = PKB::getInstance().PKB::getUsesS();
	StmtInfo s1 = getStmt(index);
	return a.containsKey(s1);
}

std::vector<std::pair<StmtInfo, var_name>> PKBAdapter::getUsesSRelation() {
	auto a = PKB::getInstance().PKB::getUsesS();
	return a.getPairs();
}

std::vector<StmtInfo> PKBAdapter::getUsesS() {
	auto a = PKB::getInstance().PKB::getUsesS();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getUsesS(var_name var) {
	auto a = PKB::getInstance().PKB::getUsesS();
	return a.getKeys(var);
}

std::vector<var_name> PKBAdapter::getUsedS(stmt_index index) {
	auto a = PKB::getInstance().PKB::getUsesS();
	StmtInfo s1 = getStmt(index);
	return a.getValues(s1);
}

bool PKBAdapter::isCallsPEmpty() {
	auto a = PKB::getInstance().PKB::getCallsP();
	return a.isEmpty();
}

bool PKBAdapter::isCallsP(proc_name proc1, proc_name proc2) {
	auto a = PKB::getInstance().PKB::getCallsP();
	return a.containsPair(proc1, proc2);
}

bool PKBAdapter::isCallerP(proc_name proc) {
	auto a = PKB::getInstance().PKB::getCallsP();
	return a.containsKey(proc);
}

bool PKBAdapter::isCalleeP(proc_name proc) {
	auto a = PKB::getInstance().PKB::getCallsP();
	return a.containsValue(proc);
}

std::vector<proc_name> PKBAdapter::getCallerP() {
	auto a = PKB::getInstance().PKB::getCallsP();
	return a.getKeys();
}

std::vector<proc_name> PKBAdapter::getCalleeP() {
	auto a = PKB::getInstance().PKB::getCallsP();
	return a.getValues();
}

std::vector<proc_name> PKBAdapter::getCallerP(proc_name proc) {
	auto a = PKB::getInstance().PKB::getCallsP();
	return a.getKeys(proc);
}

std::vector<proc_name> PKBAdapter::getCalleeP(proc_name proc) {
	auto a = PKB::getInstance().PKB::getCallsP();
	return a.getValues(proc);
}

std::vector<std::pair<proc_name, proc_name>> PKBAdapter::getCallsPRelation() {
	auto a = PKB::getInstance().PKB::getCallsP();
	return a.getPairs();
}

bool PKBAdapter::isCallsPTEmpty() {
	auto a = PKB::getInstance().PKB::getCallsPT();
	return a.isEmpty();
}

bool PKBAdapter::isCallsPT(proc_name proc1, proc_name proc2) {
	auto a = PKB::getInstance().PKB::getCallsPT();
	return a.containsPair(proc1, proc2);
}

bool PKBAdapter::isCallerPT(proc_name proc) {
	auto a = PKB::getInstance().PKB::getCallsPT();
	return a.containsKey(proc);
}

bool PKBAdapter::isCalleePT(proc_name proc) {
	auto a = PKB::getInstance().PKB::getCallsPT();
	return a.containsValue(proc);
}

std::vector<proc_name> PKBAdapter::getCallerPT() {
	auto a = PKB::getInstance().PKB::getCallsPT();
	return a.getKeys();
}

std::vector<proc_name> PKBAdapter::getCalleePT() {
	auto a = PKB::getInstance().PKB::getCallsPT();
	return a.getValues();
}

std::vector<proc_name> PKBAdapter::getCallerPT(proc_name proc) {
	auto a = PKB::getInstance().PKB::getCallsPT();
	return a.getKeys(proc);
}

std::vector<proc_name> PKBAdapter::getCalleePT(proc_name proc) {
	auto a = PKB::getInstance().PKB::getCallsPT();
	return a.getValues(proc);
}

std::vector<std::pair<proc_name, proc_name>> PKBAdapter::getCallsPTRelation() {
	auto a = PKB::getInstance().PKB::getCallsPT();
	return a.getPairs();
}

bool PKBAdapter::isNextEmpty() {
	auto a = PKB::getInstance().PKB::getNext();
	return a.isEmpty();
}

bool PKBAdapter::isNext(prog_line index1, prog_line index2) {
	auto a = PKB::getInstance().PKB::getNext();
	StmtInfo s1 = getStmt(index1);
	StmtInfo s2 = getStmt(index2);
	return a.containsPair(s1, s2);
}

bool PKBAdapter::isPrevious(prog_line index) {
	auto a = PKB::getInstance().PKB::getNext();
	StmtInfo s1 = getStmt(index);
	return a.containsKey(s1);
}

bool PKBAdapter::isNext(prog_line index) {
	auto a = PKB::getInstance().PKB::getNext();
	StmtInfo s1 = getStmt(index);
	return a.containsValue(s1);
}

std::vector<std::pair<StmtInfo, StmtInfo>> PKBAdapter::getAllNextRelation() {
	auto a = PKB::getInstance().PKB::getNext();
	return a.getPairs();
}

std::vector<StmtInfo> PKBAdapter::getPrevious() {
	auto a = PKB::getInstance().PKB::getNext();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getNext() {
	auto a = PKB::getInstance().PKB::getNext();
	return a.getValues();
}

std::vector<StmtInfo> PKBAdapter::getPrevious(prog_line index) {
	auto a = PKB::getInstance().PKB::getNext();
	StmtInfo s1 = getStmt(index);
	return a.getKeys(s1);
}

std::vector<StmtInfo> PKBAdapter::getNext(prog_line index) {
	auto a = PKB::getInstance().PKB::getNext();
	StmtInfo s1 = getStmt(index);
	return a.getValues(s1);
}

bool PKBAdapter::isIfEmpty() {
	auto a = PKB::getInstance().PKB::getIf();
	return a.isEmpty();
}

bool PKBAdapter::isIfUses(stmt_index index, var_name var) {
	auto a = PKB::getInstance().PKB::getIf();
	return a.containsPair(index, var);
}

bool PKBAdapter::isIfUses(stmt_index index) {
	auto a = PKB::getInstance().PKB::getIf();
	return a.containsKey(index);
}

bool PKBAdapter::isIfUsed(var_name var) {
	auto a = PKB::getInstance().PKB::getIf();
	return a.containsValue(var);
}

std::vector<std::pair<stmt_index, var_name>> PKBAdapter::getAllIfUses() {
	auto a = PKB::getInstance().PKB::getIf();
	return a.getPairs();
}

std::vector<stmt_index> PKBAdapter::getIfUses() {
	auto a = PKB::getInstance().PKB::getIf();
	return a.getKeys();
}

std::vector<var_name> PKBAdapter::getIfUsed() {
	auto a = PKB::getInstance().PKB::getIf();
	return a.getValues();
}

std::vector<stmt_index> PKBAdapter::getIfUses(var_name var) {
	auto a = PKB::getInstance().PKB::getIf();
	return a.getKeys(var);
}

std::vector<var_name> PKBAdapter::getIfUsed(stmt_index index) {
	auto a = PKB::getInstance().PKB::getIf();
	return a.getValues(index);
}

bool PKBAdapter::isWhileEmpty() {
	auto a = PKB::getInstance().PKB::getWhile();
	return a.isEmpty();
}

bool PKBAdapter::isWhileUses(stmt_index index, var_name var) {
	auto a = PKB::getInstance().PKB::getWhile();
	return a.containsPair(index, var);
}

bool PKBAdapter::isWhileUses(stmt_index index) {
	auto a = PKB::getInstance().PKB::getWhile();
	return a.containsKey(index);
}

bool PKBAdapter::isWhileUsed(var_name var) {
	auto a = PKB::getInstance().PKB::getWhile();
	return a.containsValue(var);
}

std::vector<std::pair<stmt_index, var_name>> PKBAdapter::getAllWhileUses() {
	auto a = PKB::getInstance().PKB::getWhile();
	return a.getPairs();
}

std::vector<stmt_index> PKBAdapter::getWhileUses() {
	auto a = PKB::getInstance().PKB::getWhile();
	return a.getKeys();
}

std::vector<var_name> PKBAdapter::getWhileUsed() {
	auto a = PKB::getInstance().PKB::getWhile();
	return a.getValues();
}

std::vector<stmt_index> PKBAdapter::getWhileUses(var_name var) {
	auto a = PKB::getInstance().PKB::getWhile();
	return a.getKeys(var);
}

std::vector<var_name> PKBAdapter::getWhileUsed(stmt_index index) {
	auto a = PKB::getInstance().PKB::getWhile();
	return a.getValues(index);
}
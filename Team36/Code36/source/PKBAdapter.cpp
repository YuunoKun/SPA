#include "PKBAdapter.h"
#include "PKB.h"
#include "Utility.h"
#include "Common.h"
#include "Stmt.h"
#include "RelationTable.h"

PKBAdapter::PKBAdapter() {
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
	std::vector<StmtInfo> v = PKB::getInstance().PKB::getStmts();
	return Utility::filterResult(ASSIGN, v);
}

std::vector<StmtInfo> PKBAdapter::getPrints() {
	std::vector<StmtInfo> v = PKB::getInstance().PKB::getStmts();
	return Utility::filterResult(PRINT, v);
}

std::vector<StmtInfo> PKBAdapter::getCalls() {
	std::vector<StmtInfo> v = PKB::getInstance().PKB::getStmts();
	return Utility::filterResult(CALL, v);
}

std::vector<StmtInfo> PKBAdapter::getReads() {
	std::vector<StmtInfo> v = PKB::getInstance().PKB::getStmts();
	return Utility::filterResult(READ, v);
}

std::vector<StmtInfo> PKBAdapter::getWhiles() {
	std::vector<StmtInfo> v = PKB::getInstance().PKB::getStmts();
	return Utility::filterResult(WHILE, v);
}

std::vector<StmtInfo> PKBAdapter::getIfs() {
	std::vector<StmtInfo> v = PKB::getInstance().PKB::getStmts();
	return Utility::filterResult(IF, v);
}


bool PKBAdapter::isFollowEmpty() {
	auto a = PKB::getInstance().PKB::getFollows();
	return a.isEmpty();
}

bool PKBAdapter::isFollow(stmt_index index1, stmt_index index2) {
	auto a = PKB::getInstance().PKB::getFollows();
	//// Need better method to find stmt_info
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index1 - 1);
	StmtInfo s2 = PKB::getInstance().PKB::getStmts().at(index2 - 1);
	return a.containsPair(s1, s2);
}

bool PKBAdapter::isFollowed(stmt_index index) {
	auto a = PKB::getInstance().PKB::getFollows();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
	return a.containsKey(s1);
}

bool PKBAdapter::isFollowing(stmt_index index) {
	auto a = PKB::getInstance().PKB::getFollowsReverse();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
	return a.containsKey(s1);
}

std::vector<StmtInfo> PKBAdapter::getFollowed() {
	auto a = PKB::getInstance().PKB::getFollows();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getFollowing() {
	auto a = PKB::getInstance().PKB::getFollowsReverse();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getFollowed(stmt_index index) {
	auto a = PKB::getInstance().PKB::getFollowsReverse();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
	return a.getValues(s1);
}

std::vector<StmtInfo> PKBAdapter::getFollowing(stmt_index index) {
	auto a = PKB::getInstance().PKB::getFollows();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
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
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index1 - 1);
	StmtInfo s2 = PKB::getInstance().PKB::getStmts().at(index2 - 1);
	return a.containsPair(s1, s2);
}

bool PKBAdapter::isFollowedT(stmt_index index) {
	auto a = PKB::getInstance().PKB::getFollowsT();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
	return a.containsKey(s1);
}

bool PKBAdapter::isFollowingT(stmt_index index) {
	auto a = PKB::getInstance().PKB::getFollowsTReverse();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
	return a.containsKey(s1);
}

std::vector<StmtInfo> PKBAdapter::getFollowedT() {
	auto a = PKB::getInstance().PKB::getFollowsT();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getFollowingT() {
	auto a = PKB::getInstance().PKB::getFollowsTReverse();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getFollowedT(stmt_index index) {
	auto a = PKB::getInstance().PKB::getFollowsTReverse();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
	return a.getValues(s1);
}

std::vector<StmtInfo> PKBAdapter::getFollowingT(stmt_index index) {
	auto a = PKB::getInstance().PKB::getFollowsT();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
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
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index1 - 1);
	StmtInfo s2 = PKB::getInstance().PKB::getStmts().at(index2 - 1);
	return a.containsPair(s1, s2);
}

bool PKBAdapter::isChild(stmt_index index) {
	auto a = PKB::getInstance().PKB::getParentReverse();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
	return a.containsKey(s1);
}

bool PKBAdapter::isParent(stmt_index index) {
	auto a = PKB::getInstance().PKB::getParent();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
	return a.containsKey(s1);
}

std::vector<StmtInfo> PKBAdapter::getChild() {
	auto a = PKB::getInstance().PKB::getParentReverse();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getParent() {
	auto a = PKB::getInstance().PKB::getParent();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getChild(stmt_index index) {
	auto a = PKB::getInstance().PKB::getParent();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
	return a.getValues(s1);
}

std::vector<StmtInfo> PKBAdapter::getParent(stmt_index index) {
	auto a = PKB::getInstance().PKB::getParentReverse();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
	return a.getValues(s1);
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
	//// Need better method to find stmt_info
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index1 - 1);
	StmtInfo s2 = PKB::getInstance().PKB::getStmts().at(index2 - 1);
	return a.containsPair(s1, s2);
}

bool PKBAdapter::isChildT(stmt_index index) {
	auto a = PKB::getInstance().PKB::getParentTReverse();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
	return a.containsKey(s1);
}

bool PKBAdapter::isParentT(stmt_index index) {
	auto a = PKB::getInstance().PKB::getParentT();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
	return a.containsKey(s1);
}

std::vector<StmtInfo> PKBAdapter::getChildT() {
	auto a = PKB::getInstance().PKB::getParentTReverse();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getParentT() {
	auto a = PKB::getInstance().PKB::getParentT();
	return a.getKeys();
}

std::vector<StmtInfo> PKBAdapter::getChildT(stmt_index index) {
	auto a = PKB::getInstance().PKB::getParentT();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
	return a.getValues(s1);
}

std::vector<StmtInfo> PKBAdapter::getParentT(stmt_index index) {
	auto a = PKB::getInstance().PKB::getParentTReverse();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
	return a.getValues(s1);
}

std::vector<std::pair<StmtInfo, StmtInfo>> PKBAdapter::getAllParentTRelation() {
	auto a = PKB::getInstance().PKB::getParentT();
	return a.getPairs();
}

bool PKBAdapter::isModifiesP(proc_name, var_name) {
	//TODO: Iteration 2
	return false;
}

bool PKBAdapter::isModifiesP(proc_name) {
	//TODO: Iteration 2
	return false;
}

std::vector<std::pair<proc_name, var_name>> PKBAdapter::getModifiesPRelation() {
	//TODO: Iteration 2
	return std::vector<std::pair<proc_name, var_name>>();
}

std::vector<proc_name> PKBAdapter::getModifiesP() {
	//TODO: Iteration 2
	return std::vector<proc_name>();
}

std::vector<proc_name> PKBAdapter::getModifiesP(var_name var) {
	//TODO: Iteration 2
	return std::vector<proc_name>();
}

std::vector<var_name> PKBAdapter::getModifiedP(proc_name proc) {
	//TODO: Iteration 2
	return std::vector<var_name>();
}

bool PKBAdapter::isModifiesSEmpty() {
	auto a = PKB::getInstance().PKB::getModifiesS();
	return a.isEmpty();
}

bool PKBAdapter::isModifiesS(stmt_index index, var_name var) {
	auto a = PKB::getInstance().PKB::getModifiesS();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
	return a.containsPair(s1, var);
}

bool PKBAdapter::isModifiesS(stmt_index index) {
	auto a = PKB::getInstance().PKB::getModifiesS();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
	return a.containsKey(s1);
}

bool PKBAdapter::isModifiesS(var_name var) {
	auto a = PKB::getInstance().PKB::getModifiesSReverse();
	return a.containsKey(var);
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
	auto a = PKB::getInstance().PKB::getModifiesSReverse();
	return a.getValues(var);
}

std::vector<var_name> PKBAdapter::getModifiedS() {
	auto a = PKB::getInstance().PKB::getModifiesSReverse();
	return a.getKeys();
}

std::vector<var_name> PKBAdapter::getModifiedS(stmt_index index) {
	auto a = PKB::getInstance().PKB::getModifiesS();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
	return a.getValues(s1);
}

bool PKBAdapter::isUsesP(proc_name proc, var_name var) {
	//TODO: Iteration 2
	return false;
}

bool PKBAdapter::isUsesP(proc_name proc) {
	//TODO: Iteration 2
	return false;
}

std::vector<std::pair<proc_name, var_name>> PKBAdapter::getUsesPRelation() {
	//TODO: Iteration 2
	return std::vector<std::pair<proc_name, var_name>>();
}

std::vector<proc_name> PKBAdapter::getUsesP() {
	//TODO: Iteration 2
	return std::vector<proc_name>();
}

std::vector<proc_name> PKBAdapter::getUsesP(var_name var) {
	//TODO: Iteration 2
	return std::vector<proc_name>();
}

std::vector<var_name> PKBAdapter::getUsedP(proc_name proc) {
	//TODO: Iteration 2
	return std::vector<var_name>();
}

bool PKBAdapter::isUsesSEmpty() {
	auto a = PKB::getInstance().PKB::getUsesS();
	return a.isEmpty();
}

bool PKBAdapter::isUsesS(stmt_index index, var_name var) {
	auto a = PKB::getInstance().PKB::getUsesS();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
	return a.containsPair(s1, var);
}

bool PKBAdapter::isUsesS(stmt_index index) {
	auto a = PKB::getInstance().PKB::getUsesS();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
	return a.containsKey(s1);
}

bool PKBAdapter::isUsesS(var_name var) {
	auto a = PKB::getInstance().PKB::getUsesSReverse();
	return a.containsKey(var);
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
	auto a = PKB::getInstance().PKB::getUsesSReverse();
	return a.getValues(var);
}

std::vector<var_name> PKBAdapter::getUsedS() {
	auto a = PKB::getInstance().PKB::getUsesSReverse();
	return a.getKeys();
}

std::vector<var_name> PKBAdapter::getUsedS(stmt_index index) {
	auto a = PKB::getInstance().PKB::getUsesS();
	StmtInfo s1 = PKB::getInstance().PKB::getStmts().at(index - 1);
	return a.getValues(s1);
}
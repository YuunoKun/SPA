#include "PKBAdapter.h"
#include "PKB.h"
#include "Utility.h"

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
	//TODO
	return false;
}

bool PKBAdapter::isFollow(stmt_index, stmt_index) {
	//TODO
	return false;
}

bool PKBAdapter::isFollowed(stmt_index) {
	//TODO
	return false;
}

bool PKBAdapter::isFollowing(stmt_index) {
	//TODO
	return false;
}

std::vector<StmtInfo> PKBAdapter::getFollowed() {
	//TODO
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> PKBAdapter::getFollowing() {
	//TODO
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> PKBAdapter::getFollowed(stmt_index) {
	//Todo
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> PKBAdapter::getFollowing(stmt_index) {
	//Todo
	return std::vector<StmtInfo>();
}

std::vector<std::pair<StmtInfo, StmtInfo>> PKBAdapter::getFollows() {
	//Todo
	return std::vector<std::pair<StmtInfo, StmtInfo>>();
}

bool PKBAdapter::isFollowTEmpty() {
	//Todo
	return false;
}

bool PKBAdapter::isFollowT(stmt_index, stmt_index) {
	//Todo
	return false;
}

bool PKBAdapter::isFollowedT(stmt_index) {
	//Todo
	return false;
}

bool PKBAdapter::isFollowingT(stmt_index) {
	//Todo
	return false;
}

std::vector<StmtInfo> PKBAdapter::getFollowedT() {
	//Todo
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> PKBAdapter::getFollowingT() {
	//Todo
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> PKBAdapter::getFollowedT(stmt_index) {
	//Todo
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> PKBAdapter::getFollowingT(stmt_index) {
	//Todo
	return std::vector<StmtInfo>();
}

std::vector<std::pair<StmtInfo, StmtInfo>> PKBAdapter::getFollowsT() {
	//Todo
	return std::vector<std::pair<StmtInfo, StmtInfo>>();
}

bool PKBAdapter::isParentEmpty() {
	//Todo
	return false;
}

bool PKBAdapter::isParent(stmt_index, stmt_index) {
	//Todo
	return false;
}

bool PKBAdapter::isChild(stmt_index) {
	//Todo
	return false;
}

bool PKBAdapter::isParent(stmt_index) {
	//Todo
	return false;
}

std::vector<StmtInfo> PKBAdapter::getChild() {
	//Todo
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> PKBAdapter::getParent() {
	//Todo
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> PKBAdapter::getChild(stmt_index) {
	//Todo
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> PKBAdapter::getParent(stmt_index) {
	//Todo
	return std::vector<StmtInfo>();
}

std::vector<std::pair<StmtInfo, StmtInfo>> PKBAdapter::getAllParentRelation() {
	//Todo
	return std::vector<std::pair<StmtInfo, StmtInfo>>();
}

bool PKBAdapter::isParentTEmpty() {
	return false;
}

bool PKBAdapter::isParentT(stmt_index, stmt_index) {
	//Todo
	return false;
}

bool PKBAdapter::isChildT(stmt_index) {
	//Todo
	return false;
}

bool PKBAdapter::isParentT(stmt_index) {
	//Todo
	return false;
}

std::vector<StmtInfo> PKBAdapter::getChildT() {
	//Todo
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> PKBAdapter::getParentT() {
	//Todo
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> PKBAdapter::getChildT(stmt_index) {
	//Todo
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> PKBAdapter::getParentT(stmt_index) {
	//Todo
	return std::vector<StmtInfo>();
}

std::vector<std::pair<StmtInfo, StmtInfo>> PKBAdapter::getAllParentTRelation() {
	//Todo
	return std::vector<std::pair<StmtInfo, StmtInfo>>();
}

bool PKBAdapter::isModifiesP(proc_name, var_name) {
	//TODO
	return false;
}

bool PKBAdapter::isModifiesP(proc_name) {
	//TODO
	return false;
}

std::vector<std::pair<std::string, std::string>> PKBAdapter::getModifiesPRelation() {
	//TODO
	return std::vector<std::pair<std::string, std::string>>();
}

std::vector<std::string> PKBAdapter::getModifiesP() {
	//TODO
	return std::vector<std::string>();
}

std::vector<std::string> PKBAdapter::getModifiesP(var_name) {
	//TODO
	return std::vector<std::string>();
}

std::vector<std::string> PKBAdapter::getModifiedP(proc_name) {
	//TODO
	return std::vector<std::string>();
}

bool PKBAdapter::isModifiesSEmpty() {
	//TODO
	return false;
}

bool PKBAdapter::isModifiesS(stmt_index, var_name) {
	//TODO
	return false;
}

bool PKBAdapter::isModifiesS(stmt_index) {
	//TODO
	return false;
}

bool PKBAdapter::isModifiesS(var_name) {
	//TODO
	return false;
}

std::vector<std::pair<StmtInfo, var_name>> PKBAdapter::getModifiesSRelation() {
	//TODO
	return std::vector<std::pair<StmtInfo, var_name>>();
}

std::vector<StmtInfo> PKBAdapter::getModifiesS() {
	//TODO
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> PKBAdapter::getModifiesS(var_name) {
	//TODO
	return std::vector<StmtInfo>();
}

std::vector<std::string> PKBAdapter::getModifiedS() {
	//TODO
	return std::vector<std::string>();
}

std::vector<std::string> PKBAdapter::getModifiedS(stmt_index) {
	//TODO
	return std::vector<std::string>();
}

bool PKBAdapter::isUsesP(proc_name, var_name) {
	//TODO
	return false;
}

bool PKBAdapter::isUsesP(proc_name) {
	//TODO
	return false;
}

std::vector<std::pair<std::string, std::string>> PKBAdapter::getUsesPRelation() {
	//TODO
	return std::vector<std::pair<std::string, std::string>>();
}

std::vector<std::string> PKBAdapter::getUsesP() {
	//TODO
	return std::vector<std::string>();
}

std::vector<std::string> PKBAdapter::getUsesP(var_name) {
	//TODO
	return std::vector<std::string>();
}

std::vector<std::string> PKBAdapter::getUsedP(proc_name) {
	//TODO
	return std::vector<std::string>();
}

bool PKBAdapter::isUsesSEmpty() {
	return false;
}

bool PKBAdapter::isUsesS(stmt_index, var_name) {
	//TODO
	return false;
}

bool PKBAdapter::isUsesS(stmt_index) {
	//TODO
	return false;
}

bool PKBAdapter::isUsesS(var_name) {
	//TODO
	return false;
}

std::vector<std::pair<StmtInfo, var_name>> PKBAdapter::getUsesSRelation() {
	//TODO
	return std::vector<std::pair<StmtInfo, var_name>>();
}

std::vector<StmtInfo> PKBAdapter::getUsesS() {
	//TODO
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> PKBAdapter::getUsesS(var_name) {
	//TODO
	return std::vector<StmtInfo>();
}

std::vector<std::string> PKBAdapter::getUsedS() {
	//TODO
	return std::vector<std::string>();
}

std::vector<std::string> PKBAdapter::getUsedS(stmt_index) {
	//TODO
	return std::vector<std::string>();
}
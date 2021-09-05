#include "PKBAdapter.h"
#include "PKB.h"
#include "Utility.h"

PKBAdapter::PKBAdapter() {
}

std::vector<var_name> PKBAdapter::getVariables() {
	return PKB::getInstance().PKB::getVariables();
}

std::vector<StmtInfo> PKBAdapter::getStmts() {
	return Utility::stmtToStmtInfoVector(PKB::getInstance().PKB::getStmts());
}

std::vector<constant> PKBAdapter::getConstants() {
	return PKB::getInstance().getConstants();
}

std::vector<proc_name> PKBAdapter::getProcedures() {
	return PKB::getInstance().PKB::getProcedures();
}

std::vector<StmtInfo> PKBAdapter::getAssigns() {
	//TODO
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> PKBAdapter::getPrints() {
	//TODO
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> PKBAdapter::getCalls() {
	//TODO
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> PKBAdapter::getReads() {
	//TODO
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> PKBAdapter::getWhiles() {
	//TODO
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> PKBAdapter::getIfs() {
	//TODO
	return std::vector<StmtInfo>();
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

std::vector<std::vector<StmtInfo>> PKBAdapter::getFollows() {
	//Todo
	return std::vector<std::vector<StmtInfo>>();
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

std::vector<std::vector<StmtInfo>> PKBAdapter::getFollowsT() {
	//Todo
	return std::vector<std::vector<StmtInfo>>();
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

std::vector<std::vector<StmtInfo>> PKBAdapter::getAllParentRelation() {
	//Todo
	return std::vector<std::vector<StmtInfo>>();
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

std::vector<std::vector<StmtInfo>> PKBAdapter::getAllParentTRelation() {
	//Todo
	return std::vector<std::vector<StmtInfo>>();
}

bool PKBAdapter::isModifies(proc_name, var_name) {
	//TODO
	return false;
}

bool PKBAdapter::isModifies(proc_name) {
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

bool PKBAdapter::isModifies(stmt_index, var_name) {
	//TODO
	return false;
}

bool PKBAdapter::isModifies(stmt_index) {
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

std::vector<std::string> PKBAdapter::getModifiedS(stmt_index) {
	//TODO
	return std::vector<std::string>();
}

bool PKBAdapter::isUses(proc_name, var_name) {
	//TODO
	return false;
}

bool PKBAdapter::isUses(proc_name) {
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

bool PKBAdapter::isUses(stmt_index, var_name) {
	//TODO
	return false;
}

bool PKBAdapter::isUses(stmt_index) {
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

std::vector<std::string> PKBAdapter::getUsedS(stmt_index) {
	//TODO
	return std::vector<std::string>();
}

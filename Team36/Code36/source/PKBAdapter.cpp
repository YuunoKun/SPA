#include "PKBAdapter.h"
#include "PKB.h"

PKBAdapter::PKBAdapter() {
}

std::vector<var_name> PKBAdapter::getVariables() {
	return PKB::getInstance().PKB::getVariables();
}

std::vector<Stmt> PKBAdapter::getStmts() {
	return PKB::getInstance().PKB::getStmts();
}

std::vector<constant> PKBAdapter::getConstants() {
	return PKB::getInstance().getConstants();
}

std::vector<proc_name> PKBAdapter::getProcedures() {
	return PKB::getInstance().PKB::getProcedures();
}

std::vector<Stmt> PKBAdapter::getAssigns() {
	//TODO
	return std::vector<Stmt>();
}

std::vector<Stmt> PKBAdapter::getPrints() {
	//TODO
	return std::vector<Stmt>();
}

std::vector<Stmt> PKBAdapter::getCalls() {
	//TODO
	return std::vector<Stmt>();
}

std::vector<Stmt> PKBAdapter::getReads() {
	//TODO
	return std::vector<Stmt>();
}

std::vector<Stmt> PKBAdapter::getWhiles() {
	//TODO
	return std::vector<Stmt>();
}

std::vector<Stmt> PKBAdapter::getIfs() {
	//TODO
	return std::vector<Stmt>();
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

std::vector<Stmt> PKBAdapter::getFollowed() {
	//TODO
	return std::vector<Stmt>();
}

std::vector<Stmt> PKBAdapter::getFollowing() {
	//TODO
	return std::vector<Stmt>();
}

std::vector<Stmt> PKBAdapter::getFollowed(stmt_index) {
	//Todo
	return std::vector<Stmt>();
}

std::vector<Stmt> PKBAdapter::getFollowing(stmt_index) {
	//Todo
	return std::vector<Stmt>();
}

std::vector<std::vector<Stmt>> PKBAdapter::getFollows() {
	//Todo
	return std::vector<std::vector<Stmt>>();
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

std::vector<Stmt> PKBAdapter::getFollowedT() {
	//Todo
	return std::vector<Stmt>();
}

std::vector<Stmt> PKBAdapter::getFollowingT() {
	//Todo
	return std::vector<Stmt>();
}

std::vector<Stmt> PKBAdapter::getFollowedT(stmt_index) {
	//Todo
	return std::vector<Stmt>();
}

std::vector<Stmt> PKBAdapter::getFollowingT(stmt_index) {
	//Todo
	return std::vector<Stmt>();
}

std::vector<std::vector<Stmt>> PKBAdapter::getFollowsT() {
	//Todo
	return std::vector<std::vector<Stmt>>();
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

std::vector<Stmt> PKBAdapter::getChild() {
	//Todo
	return std::vector<Stmt>();
}

std::vector<Stmt> PKBAdapter::getParent() {
	//Todo
	return std::vector<Stmt>();
}

std::vector<Stmt> PKBAdapter::getChild(stmt_index) {
	//Todo
	return std::vector<Stmt>();
}

std::vector<Stmt> PKBAdapter::getParent(stmt_index) {
	//Todo
	return std::vector<Stmt>();
}

std::vector<std::vector<Stmt>> PKBAdapter::getAllParentRelation() {
	//Todo
	return std::vector<std::vector<Stmt>>();
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

std::vector<Stmt> PKBAdapter::getChildT() {
	//Todo
	return std::vector<Stmt>();
}

std::vector<Stmt> PKBAdapter::getParentT() {
	//Todo
	return std::vector<Stmt>();
}

std::vector<Stmt> PKBAdapter::getChildT(stmt_index) {
	//Todo
	return std::vector<Stmt>();
}

std::vector<Stmt> PKBAdapter::getParentT(stmt_index) {
	//Todo
	return std::vector<Stmt>();
}

std::vector<std::vector<Stmt>> PKBAdapter::getAllParentTRelation() {
	//Todo
	return std::vector<std::vector<Stmt>>();
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

std::vector<std::pair<Stmt, var_name>> PKBAdapter::getModifiesSRelation() {
	//TODO
	return std::vector<std::pair<Stmt, var_name>>();
}

std::vector<Stmt> PKBAdapter::getModifiesS() {
	//TODO
	return std::vector<Stmt>();
}

std::vector<Stmt> PKBAdapter::getModifiesS(var_name) {
	//TODO
	return std::vector<Stmt>();
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

std::vector<std::pair<Stmt, var_name>> PKBAdapter::getUsesSRelation() {
	//TODO
	return std::vector<std::pair<Stmt, var_name>>();
}

std::vector<Stmt> PKBAdapter::getUsesS() {
	//TODO
	return std::vector<Stmt>();
}

std::vector<Stmt> PKBAdapter::getUsesS(var_name) {
	//TODO
	return std::vector<Stmt>();
}

std::vector<std::string> PKBAdapter::getUsedS(stmt_index) {
	//TODO
	return std::vector<std::string>();
}

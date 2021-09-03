#include "PKBAdapter.h"
#include "PKB.h"

PKBAdapter::PKBAdapter() {
}

std::vector<variable_name> PKBAdapter::getVariables() {
	return PKB::getInstance().PKB::getVariables();
}

std::vector<Stmt> PKBAdapter::getStmts() {
	return PKB::getInstance().PKB::getStmts();
}

std::vector<constant> PKBAdapter::getConstants() {
	return PKB::getInstance().getConstants();
}

std::vector<procedure_name> PKBAdapter::getProcedures() {
	return PKB::getInstance().PKB::getProcedures();
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

std::vector<std::string> PKBAdapter::getFollowed() {
	//TODO
	return std::vector<std::string>();
}

std::vector<std::string> PKBAdapter::getFollowing() {
	//TODO
	return std::vector<std::string>();
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
	return std::vector<std::vector<Stmt>>();
}

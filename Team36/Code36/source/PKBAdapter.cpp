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
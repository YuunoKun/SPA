#include "Statement.h"


using namespace SourceProcessor;


Statement::Statement(stmt_index id, StmtType type, proc_name parent_proc_name, int stmt_list) {
	statement_index = id;
	statement_type = type;
	under_procedure = parent_proc_name;
	statement_direct_parent = 0;
	statement_list_no = stmt_list;
}


StmtType Statement::getType() {
	return statement_type;
}


stmt_index Statement::getIndex() {
	return statement_index;
}


proc_name Statement::getProcName() {
	return under_procedure;
}


void Statement::setDirectParent(stmt_index parent) {
	statement_direct_parent = parent;
}


stmt_index Statement::getDirectParent() {
	return statement_direct_parent;
}


void Statement::addDirectChild(stmt_index child) {
	statement_direct_child.push_back(child);
}


std::vector<stmt_index>& Statement::getDirectChild() {
	return statement_direct_child;
}


int Statement::getStmtList() {
	return statement_list_no;
}


void Statement::addUsesVariable(var_name var) {
	statement_uses.insert(var);
}


std::vector<var_name> Statement::getUsedVariable() {
	return std::vector<var_name>(statement_uses.begin(), statement_uses.end());
}


void Statement::addModifiesVariable(var_name var) {
	statement_modifies.insert(var);
}


std::vector<var_name> Statement::getModifiedVariable() {
	return std::vector<var_name>(statement_modifies.begin(), statement_modifies.end());
}


void Statement::setExprStr(std::string expr) {
	statement_expr = expr;
}


std::string Statement::getExprStr() {
	return statement_expr;
}


void Statement::setCallee(proc_name name) {
	statement_callee = name;
}


proc_name Statement::getCallee() {
	return statement_callee;
}
#include "Statement.h"


using namespace SourceProcessor;


Statement::Statement(stmt_index id, StmtType type, proc_name parent_proc_name) {
	m_index = id;
	m_type = type;
	m_under_procedure = parent_proc_name;
	m_direct_parent = 0;
}


StmtType Statement::get_type() {
	return m_type;
}


stmt_index Statement::get_index() {
	return m_index;
}


void Statement::add_direct_parent(stmt_index parent) {
	m_direct_parent = parent;
}


proc_name Statement::get_direct_parent() {
	return m_direct_parent;
}


void Statement::add_direct_child(stmt_index child) {
	m_direct_child.insert(child);
}


std::vector<stmt_index>& Statement::get_direct_child() {
	return *m_direct_child;
}


void Statement::add_uses_variable(var_name var) {
	m_uses.insert(var);
}


std::vector<var_name> Statement::get_used_variable() {
	return std::vector<stmt_index>(m_uses.begin(), m_uses.end());
}


void Statement::add_modifies_variable(var_name var) {
	m_modifies.insert(var);
}


std::vector<var_name> Statement::get_modified_variable() {
	return std::vector<stmt_index>(m_modifies.begin(), m_modifies.end());
}


void Statement::set_expr_str(std::string expr) {
	m_expr = expr;
}


std::string Statement::get_expr_str() {
	return m_expr;
}


void Statement::set_callee(proc_name name) {
	m_callee = name;
}


procedure_name Statement::get_callee() {
	return m_callee;
}
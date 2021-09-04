#include "Statement.h"


using namespace SourceProcessor;


Statement::Statement(stmt_index id, StmtType type, procedure_name parent_proc_name) {
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


procedure_name Statement::get_direct_parent() {
	return m_direct_parent;
}


void Statement::add_direct_child(stmt_index child) {
	m_direct_child.insert(child);
}


std::vector<stmt_index>& Statement::get_direct_child() {
	return *m_direct_child;
}


void Statement::add_uses_variable(variable_name var) {
	m_uses.insert(var);
}


std::vector<variable_name> Statement::get_used_variable() {
	return std::vector<stmt_index>(m_uses.begin(), m_uses.end());
}


void Statement::add_modifies_variable(variable_name var) {
	m_modifies.insert(var);
}


std::vector<variable_name> Statement::get_modified_variable() {
	return std::vector<stmt_index>(m_modifies.begin(), m_modifies.end());
}
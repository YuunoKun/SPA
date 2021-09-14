#include "Procedure.h"


using namespace SourceProcessor;


Procedure::Procedure(proc_name name, proc_index id) {
	m_name = name;
	m_index = id;
}


proc_name Procedure::get_name() {
	return m_name;
}


proc_index Procedure::get_index() {
	return m_index;
}


void Procedure::add_caller(stmt_index id) {
	m_caller.insert(id);
}


std::vector<stmt_index> Procedure::get_caller() {
	return std::vector<stmt_index>(m_caller.begin(), m_caller.end());
}


void Procedure::add_child(stmt_index id) {
	m_contained_stmt.push_back(id);
}


std::vector<stmt_index> Procedure::get_child() {
	return m_contained_stmt;
}


void Procedure::add_uses_variable(var_name name) {
	m_uses.insert(name);
}


std::vector<var_name> Procedure::get_used_variable() {
	return std::vector<var_name>(m_uses.begin(), m_uses.end());
}


void Procedure::add_modifies_variable(var_name name) {
	m_modifies.insert(name);
}


std::vector<var_name> Procedure::get_modified_variable() {
	return std::vector<var_name>(m_modifies.begin(), m_modifies.end());
}
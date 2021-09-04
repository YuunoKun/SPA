#include "Procedure.h"


using namespace SourceProcessor;


Procedure::Procedure(procedure_name name, procedure_index id) {
	m_name = name;
	m_index = id;
}


procedure_name Procedure::get_name() {
	return m_name;
}


procedure_index Procedure::get_index() {
	return m_index;
}


void Procedure::add_caller(stmt_index id) {
	m_caller.insert(id);
}


std::vector<stmt_index> Procedure::get_caller() {
	return std::vector<stmt_index>(m_caller.begin(), m_caller.end());
}


void Procedure::add_uses_variable(variable_name name) {
	m_uses.insert(name);
}


std::vector<stmt_index> Procedure::get_used_variable() {
	return std::vector<stmt_index>(m_uses.begin(), m_uses.end());
}


void Procedure::add_modifies_variable(variable_name name) {
	m_modifies.insert(name);
}


std::vector<stmt_index> Procedure::get_modified_variable() {
	return std::vector<stmt_index>(m_modifies.begin(), m_modifies.end());
}
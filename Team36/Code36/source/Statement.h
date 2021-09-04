#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include "Common.h"


namespace SourceProcessor {

	class Statement {
	public:

		Statement(stmt_index, StmtType, procedure_name);

		StmtType get_type();

		stmt_index get_index();

		void add_direct_parent(stmt_index);

		procedure_name get_direct_parent();

		void add_direct_child(stmt_index);

		std::vector<stmt_index>& get_direct_child();

		void add_uses_variable(variable_name);

		std::vector<variable_name> get_used_variable();

		void add_modifies_variable(variable_name);

		std::vector<variable_name> get_modified_variable();

	private:
		stmt_index m_index;
		StmtType m_type;
		procedure_name m_under_procedure;
		stmt_index m_direct_parent;
		std::vector<stmt_index> m_direct_child;
		std::unordered_set<variable_name> m_uses;
		std::unordered_set<variable_name> m_modifies;
	};

} // SourceProcessor
#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include "Common.h"


namespace SourceProcessor {

	class Statement {
	public:

		Statement(stmt_index, StmtType, proc_name);

		StmtType get_type();

		stmt_index get_index();

		void add_direct_parent(stmt_index);

		stmt_index get_direct_parent();

		void add_direct_child(stmt_index);

		std::vector<stmt_index>& get_direct_child();

		void add_uses_variable(var_name);

		std::vector<var_name> get_used_variable();

		void add_modifies_variable(var_name);

		std::vector<var_name> get_modified_variable();

		void set_expr_str(std::string);

		std::string get_expr_str();

		void set_callee(proc_name);

		proc_name get_callee();


	private:
		stmt_index m_index;
		StmtType m_type;
		proc_name m_under_procedure;
		stmt_index m_direct_parent;
		std::vector<stmt_index> m_direct_child;
		std::unordered_set<var_name> m_uses;
		std::unordered_set<var_name> m_modifies;
		std::string m_expr;
		proc_name m_callee;
	};

} // SourceProcessor
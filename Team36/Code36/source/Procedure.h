#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <set>
#include "Common.h"


namespace SourceProcessor {

	class Procedure {
	public:

		Procedure(proc_name, proc_index);

		proc_name get_name();

		proc_index get_index();

		void add_caller(stmt_index);

		std::vector<stmt_index> get_caller();

		void add_uses_variable(var_name);

		std::vector<stmt_index> get_used_variable();

		void add_modifies_variable(var_name);

		std::vector<stmt_index> get_modified_variable();

	private:
		proc_name m_name;
		proc_index m_index;
		std::set<stmt_index> m_caller;
		std::unordered_set<var_name> m_uses;
		std::unordered_set<var_name> m_modifies;
	};

} // SourceProcessor
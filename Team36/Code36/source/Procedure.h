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

		proc_name getName();

		proc_index getIndex();

		void addCaller(stmt_index);

		std::vector<stmt_index> getCaller();

		void addChild(stmt_index);

		std::vector<stmt_index> getChild();

		void addUsesVariable(var_name);

		std::vector<var_name> getUsedVariable();

		void addModifiesVariable(var_name);

		std::vector<var_name> getModifiedVariable();

	private:
		proc_name procedure_name;
		proc_index procedure_index;
		std::set<stmt_index> procedure_caller;
		std::vector<stmt_index> procedure_contained_stmt;
		std::unordered_set<var_name> procedure_uses;
		std::unordered_set<var_name> procedure_modifies;
	};

} // SourceProcessor
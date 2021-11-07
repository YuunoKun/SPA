#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <set>
#include "SPCommon.h"

namespace SourceProcessor {

	class Procedure {
	public:
		Procedure(proc_name, proc_index);

		proc_name getName();
		proc_index getIndex();
		std::vector<stmt_index> getCaller();
		std::vector<stmt_index> getChild();
		std::vector<var_name> getUsedVariable();
		std::vector<var_name> getModifiedVariable();

		void addCaller(stmt_index);
		void addChild(stmt_index);
		void addUsesVariable(var_name);
		void addModifiesVariable(var_name);

	private:
		proc_name procedure_name;
		proc_index procedure_index;
		std::set<stmt_index> procedure_caller;
		std::vector<stmt_index> procedure_contained_stmt;
		std::unordered_set<var_name> procedure_uses;
		std::unordered_set<var_name> procedure_modifies;
	};

} // SourceProcessor

#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include "Common.h"


namespace SourceProcessor {

	class Statement {
	public:

		Statement(stmt_index, StmtType, proc_name, int);

		StmtType getType();

		stmt_index getIndex();

		proc_name getProcName();

		void setDirectParent(stmt_index);

		stmt_index getDirectParent();

		void addDirectChild(stmt_index);

		std::vector<stmt_index>& getDirectChild();

		int getStmtList();

		void addUsesVariable(var_name);

		std::vector<var_name> getUsedVariable();

		void addModifiesVariable(var_name);

		std::vector<var_name> getModifiedVariable();

		void addUsesCondVariable(var_name);

		std::vector<var_name> getUsedCondVariable();

		void setExprStr(std::string);

		std::string getExprStr();

		void setCallee(proc_name);

		proc_name getCallee();


	private:
		stmt_index statement_index;
		StmtType statement_type;
		proc_name under_procedure;
		stmt_index statement_direct_parent{ 0 };
		std::vector<stmt_index> statement_direct_child;
		int statement_list_no{ 0 };
		std::unordered_set<var_name> statement_uses;
		std::unordered_set<var_name> statement_modifies;
		std::unordered_set<var_name> statement_uses_cond;
		std::string statement_expr;
		proc_name statement_callee;
	};

} // SourceProcessor
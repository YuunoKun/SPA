#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include "SPCommon.h"

namespace SourceProcessor {

	class Statement {
	public:
		Statement(stmt_index, StmtType, proc_name, int);

		StmtType getType();
		stmt_index getIndex();
		proc_name getProcName();
		stmt_index getDirectParent();
		std::vector<stmt_index>& getDirectChild();
		int getStmtList();
		std::vector<var_name> getUsedVariable();
		std::vector<var_name> getModifiedVariable();
		std::vector<var_name> getUsedCondVariable();
		std::string getExprStr();
		proc_name getCallee();

		void setDirectParent(stmt_index);
		void addDirectChild(stmt_index);
		void addUsesVariable(var_name);
		void addModifiesVariable(var_name);
		void addUsesCondVariable(var_name);
		void setExprStr(std::string);
		void setCallee(proc_name);

	private:
		StmtType statement_type;
		stmt_index statement_index;
		proc_name under_procedure;
		stmt_index statement_direct_parent{ INIT_DIRECT_PARENT };
		std::vector<stmt_index> statement_direct_child;
		int statement_list_no{ INIT_STMT_LIST_NO };
		std::unordered_set<var_name> statement_uses;
		std::unordered_set<var_name> statement_modifies;
		std::unordered_set<var_name> statement_uses_cond;
		std::string statement_expr;
		proc_name statement_callee;
	};

} // SourceProcessor

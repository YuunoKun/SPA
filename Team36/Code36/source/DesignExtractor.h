#pragma once

#include <vector>
#include <stack>
#include <sstream>
#include <unordered_set>
#include "Common.h"
#include "Procedure.h"
#include "Statement.h"
#include "Token.h"



namespace SourceProcessor {

	class DesignExtractor {
	public:

		DesignExtractor();

		void start_nesting();

		void end_nesting();

		void add_procedure(proc_name);

		void add_statement(TokenType);

		void add_variable(var_name);

		void add_constant(constant);

		void add_statement_uses(var_name);

		void add_statement_modifies(var_name);

		void start_expr();

		void end_expr();

		void populateEntities();

		void populateRelations();


	private:
		std::vector<Procedure> m_procedures;
		std::vector<Statement> m_statements;
		std::unordered_set<var_name> m_variables;
		std::unordered_set<constant> m_constants;

		proc_index m_curr_proc_id;
		stmt_index m_curr_stmt_id;
		std::stack<stmt_index> m_curr_nesting_stk;
		std::stringstream m_expr_builder;


		void populateFollows();
		void populateFollowsT();
		void populateParent();
		void populateParentT();
		void populateUses();
		void populateModifies();

	};

} // namespace SourceProcessor
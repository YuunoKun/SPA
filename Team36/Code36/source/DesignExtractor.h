#pragma once

#include <vector>
#include <stack>
#include "Extractor.h"
#include <unordered_set>
#include "Procedure.h"
#include "Statement.h"



namespace SourceProcessor {

	class DesignExtractor : public Extractor {
	public:

		DesignExtractor();

		void start_nesting() override;

		void end_nesting() override;

		void add_procedure(proc_name) override;

		void add_statement(TokenType) override;

		void add_variable(var_name) override;

		void add_constant(constant) override;

		void add_statement_uses(var_name) override;

		void add_statement_modifies(var_name) override;

		void start_expr() override;

		void end_expr() override;

		void populateEntities() override;

		void populateRelations() override;


	private:
		std::vector<Procedure> m_procedures;
		std::vector<Statement> m_statements;
		std::unordered_set<var_name> m_variables;
		std::unordered_set<constant> m_constants;

		proc_index m_curr_proc_id;
		stmt_index m_curr_stmt_id;
		std::stack<stmt_index> m_curr_nesting_stk;
		std::string m_expr_builder;


		void populateFollows();
		void populateFollowsT();
		void populateParent();
		void populateParentT();
		void populateUses();
		void populateModifies();

	};

} // namespace SourceProcessor
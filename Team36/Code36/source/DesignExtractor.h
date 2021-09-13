#pragma once

#include <vector>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include "Extractor.h"
#include "Procedure.h"
#include "Statement.h"



namespace SourceProcessor {

	class DesignExtractor : public Extractor {
	public:

		DesignExtractor();

		~DesignExtractor() override;

		void start_nesting() override;

		void chop_nesting() override;

		void end_nesting() override;

		void add_procedure(proc_name) override;

		const std::vector<Procedure*>& get_procedures();

		void add_statement(TokenType) override;

		const std::vector<Statement*>& get_statements();

		void add_variable(var_name) override;

		const std::unordered_set<var_name>& get_variables();

		void add_constant(constant) override;

		const std::unordered_set<constant>& get_constants();

		void add_statement_uses(var_name) override;

		void add_statement_modifies(var_name) override;

		void start_expr() override;

		void add_expr_segment(std::string) override;

		void end_expr() override;

		void add_callee(proc_name) override;

		void validate() override;

		void populate_post_validation();

		void populateEntities() override;

		void populateRelations() override;


	private:
		std::vector<Procedure*> m_procedures;
		std::vector<Statement*> m_statements;
		std::unordered_set<var_name> m_variables;
		std::unordered_set<constant> m_constants;
		std::unordered_map<proc_name, proc_index> m_proc_name_to_id;
		proc_index m_curr_proc_id;
		stmt_index m_curr_stmt_id;
		std::stack<stmt_index> m_curr_nesting_stk;
		std::string m_expr_builder;
		int m_stmt_list_id;
		std::vector<proc_index> m_call_sequence;

		void populateProcedures();
		void populateStatements();
		void populateVariables();
		void populateConstants();

		void populateFollows();
		void populateParent();
		void populateUses();
		void populateModifies();

	};

} // namespace SourceProcessor
#include <vector>
#include <stack>
#include "Common.h"
#include "Procedure.h"
#include "Statement.h"



namespace SourceProcessor {

	class DesignExtractor {

	public:

		DesignExtractor();

		void start_nesting();

		void end_nesting();

		void add_procedure(procedure_name);

		void add_statement(StmtType);

		void add_statement_uses(variable_name);

		void add_statement_modifies(variable_name);

		void start_expr();

		void end_expr();

		void populateRelations();


	private:
		std::vector<Procedure> m_procedures;
		std::vector<Statement> m_statements;
		std::vector<variable_name> m_variables;
		std::vector<constant> m_constants;

		procedure_index m_curr_proc_id;
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
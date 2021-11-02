#pragma once

#include <vector>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include "SPCommon.h"
#include "CFG.h"
#include "Extractor.h"
#include "Procedure.h"
#include "Statement.h"
#include "ExprParser.h"

namespace SourceProcessor {
	class DesignExtractor : public Extractor {
	public:
		DesignExtractor();

		~DesignExtractor();

		void startNesting() override;
		void chopNesting() override;
		void endNesting() override;
		void setCondExpr(bool) override;
		void addProcedure(proc_name) override;
		void addStatement(TokenType) override;
		void addVariable(var_name) override;
		void addConstant(constant) override;
		const std::vector<Procedure*>& getProcedures();
		const std::vector<Statement*>& getStatements();
		const std::unordered_set<var_name>& getVariables();
		const std::unordered_set<constant>& getConstants();
		void addStatementUses(var_name) override;
		void addStatementModifies(var_name) override;
		void startExpr() override;
		void addExprSegment(std::string) override;
		void endExpr() override;
		void addCallee(proc_name) override;

		void validate() override;

		void populatePostValidation() override;
		void populateEntities(PKB&) override;
		void populateRelations(PKB&) override;

	private:
		std::vector<Procedure*> de_procedures;
		std::vector<Statement*> de_statements;
		std::unordered_set<var_name> de_variables;
		std::unordered_set<constant> de_constants;
		std::unordered_map<proc_name, proc_index> proc_name_to_id;
		proc_index curr_proc_id;
		stmt_index curr_stmt_id;
		std::stack<stmt_index> curr_nesting_stk;
		std::string expr_builder;
		int curr_stmt_list_id;
		std::vector<proc_index> call_sequence;
		bool is_cond_expr;

		void populateProcedures(PKB&);
		void populateStatements(PKB&);
		void populateVariables(PKB&);
		void populateConstants(PKB&);

		void populateFollows(PKB&);
		void populateParent(PKB&);
		void populateUses(PKB&);
		void populateModifies(PKB&);
		void populateCalls(PKB&);
		void populateNext(PKB&);

		void populateIfs(PKB&);
		void populateWhiles(PKB&);

		CFG* generateCFG(std::vector<stmt_index>);
	};

} // namespace SourceProcessor

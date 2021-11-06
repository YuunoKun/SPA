#pragma once

#include <vector>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include "SPCommon.h"
#include "Parser.h"
#include "CFG.h"
#include "Extractor.h"
#include "Procedure.h"
#include "Statement.h"
#include "ExprParser.h"

namespace SourceProcessor {
	class DesignExtractor : public Extractor {
	public:
		DesignExtractor() = delete;

		DesignExtractor(PKBSourceInterface&);

		~DesignExtractor();

		void startNesting();
		void chopNesting();
		void endNesting();
		void setCondExpr(bool);
		void addProcedure(proc_name);
		void addStatement(TokenType);
		void addVariable(var_name);
		void addConstant(constant);
		const std::vector<Procedure*>& getProcedures() const;
		const std::vector<Statement*>& getStatements() const;
		const std::unordered_set<var_name>& getVariables() const;
		const std::unordered_set<constant>& getConstants() const;
		void addStatementUses(var_name);
		void addStatementModifies(var_name);
		void startExpr();
		void addExprSegment(std::string);
		void endExpr();
		void addCallee(proc_name);

		void validate();

		void populatePostValidation();
		void populateEntities();
		void populateRelations();

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

		void populateProcedures();
		void populateStatements();
		void populateVariables();
		void populateConstants();

		void populateFollows();
		void populateParent();
		void populateUses();
		void populateModifies();
		void populateCalls();
		void populateNext();

		void populateIfs();
		void populateWhiles();

		CFG* generateCFG(std::vector<stmt_index>);

	};

} // namespace SourceProcessor

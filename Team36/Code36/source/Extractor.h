#pragma once

#include "Common.h"
#include "Token.h"

namespace SourceProcessor {

	class Extractor {
	public:
		virtual ~Extractor() {}

		virtual void start_nesting() = 0;

		virtual void end_nesting() = 0;

		virtual void add_procedure(proc_name) = 0;

		virtual void add_statement(TokenType) = 0;

		virtual void add_variable(var_name) = 0;

		virtual void add_constant(constant) = 0;

		virtual void add_statement_uses(var_name) = 0;

		virtual void add_statement_modifies(var_name) = 0;

		virtual void start_expr() = 0;

		virtual void end_expr() = 0;

		virtual void populateEntities() = 0;

		virtual void populateRelations() = 0;
	};

} // namespace SourceProcessor
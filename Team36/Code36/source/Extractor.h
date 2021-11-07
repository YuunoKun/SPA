#pragma once

#include <string>
#include "SPCommon.h"
#include "Token.h"
#include "PKBSourceInterface.h"

namespace SourceProcessor {

	class Extractor {
	public:
		Extractor() = delete;

		Extractor(PKBSourceInterface& instance) : pkb_instance(instance) {}

		virtual void startNesting() = 0;
		virtual void chopNesting() = 0;
		virtual void endNesting() = 0;
		virtual void setCondExpr(bool) = 0;
		virtual void addProcedure(proc_name) = 0;
		virtual void addStatement(TokenType) = 0;
		virtual void addVariable(var_name) = 0;
		virtual void addConstant(constant) = 0;
		virtual void addStatementUses(var_name) = 0;
		virtual void addStatementModifies(var_name) = 0;
		virtual void startExpr() = 0;
		virtual void addExprSegment(std::string) = 0;
		virtual void endExpr() = 0;
		virtual void addCallee(proc_name) = 0;

		virtual void validate() = 0;

		virtual void populatePostValidation() = 0;
		virtual void populateEntities() = 0;
		virtual void populateRelations() = 0;

	protected:
		PKBSourceInterface& pkb_instance;
	};

} // namespace SourceProcessor

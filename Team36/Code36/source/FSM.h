#pragma once

#include <stdexcept>
#include <string>
#include "PKB.h"
#include "Tokenizer.h"
#include "Common.h"
#include "DesignExtractor.h"


namespace SourceProcessor {

	class FSM {
	public:
		FSM(Tokenizer&);

		// Build data structure with the given tokenizer
		void build();

	private:
		Tokenizer m_tokenizer;
		DesignExtractor m_design_extractor;

		// Expects a procedure.
		// procedure: ¡®procedure¡¯ proc_name ¡®{¡® stmtLst ¡®}¡¯
		void expect_procedure();

		// Expects a statement list.
		// stmtLst: stmt+
		void expect_statement_list();

		// Expects a statement.
		// stmt: read | print | call | while | if | assign
		void expect_statement();

		// Expects a read statement.
		// read: ¡®read¡¯ var_name¡¯;¡¯
		void expect_statement_type_read();

		// Expects a print statement.
		// print: ¡®print¡¯ var_name¡¯;¡¯
		void expect_statement_type_print();

		// Expects a call statement.
		// call: ¡®call¡¯ proc_name ¡®;¡¯
		void expect_statement_type_call();

		// Expects a while container statement.
		// while: ¡®while¡¯ ¡®(¡¯ cond_expr ¡®)¡¯ ¡®{¡® stmtLst ¡®}¡¯
		void expect_statement_type_while();

		// Expects a if container statement.
		// if: ¡®if¡¯ ¡®(¡¯ cond_expr ¡®)¡¯ ¡®then¡¯ ¡®{¡® stmtLst ¡®}¡¯ ¡®else¡¯ ¡®{¡® stmtLst ¡®}¡¯
		void expect_statement_type_if();

		// Expects a assignment statement.
		// assign: var_name ¡®=¡¯ expr ¡®;¡¯
		void expect_statement_type_assign();

		// Expects a conditional expression.
		// cond_expr: rel_expr | ¡®!¡¯ ¡®(¡¯ cond_expr ¡®)¡¯ | ¡®(¡¯ cond_expr ¡®)¡¯ ¡®&&¡¯ ¡®(¡¯ cond_expr ¡®)¡¯ | ¡®(¡¯ cond_expr ¡®)¡¯ ¡®||¡¯ ¡®(¡¯ cond_expr ¡®)¡¯
		void expect_conditional_expression();

		// Expects a relational expression.
		// rel_expr: rel_factor ¡®>¡¯ rel_factor | rel_factor ¡®>=¡¯ rel_factor | rel_factor ¡®<¡¯ rel_factor | rel_factor ¡®<=¡¯ rel_factor | rel_factor ¡®==¡¯ rel_factor | rel_factor ¡®!=¡¯ rel_factor
		void expect_relational_expression();

		// Expects a replational factor.
		// rel_factor: var_name | const_value | expr
		void expect_relational_factor();

		// Expects a expression.
		// expr: expr ¡®+¡¯ term | expr ¡®-¡¯ term | term
		void expect_expression();

		// Expects a term.
		// term: term ¡®*¡¯ factor | term ¡®/¡¯ factor | term ¡®%¡¯ factor | factor
		void expect_term();

		// Expects a factor.
		// factor: var_name | const_value | ¡®(¡¯ expr ¡®)¡¯
		void expect_factor();

		// Expects a compulsory token with given token type, or else the build fails.
		Token& expect_token_and_pop(TokenType);

		// Throws exception
		void unexpected_token();

		// This token can be optional, will return a boolean value indicating if that token exists
		bool optional_token(TokenType);
	};


	typedef enum BUILD_STATUS {
		INIT,
		SUCCESS,
		FAILURE,
		INVALID
	} BuildStatus;

} // namespace SourceProcessor
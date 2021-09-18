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

		FSM(Tokenizer&, Extractor*);

		Tokenizer& getTokenizer();

		// Build data structure with the given tokenizer
		void build();

		// Expects a procedure.
		// procedure: ¡®procedure¡¯ proc_name ¡®{¡® stmtLst ¡®}¡¯
		void expectProcedure();

		// Expects a statement list.
		// stmtLst: stmt+
		void expectStatementList();

		// Expects a statement.
		// stmt: read | print | call | while | if | assign
		void expectStatement();

		// Expects a read statement.
		// read: ¡®read¡¯ var_name¡¯;¡¯
		void expectStatementTypeRead();

		// Expects a print statement.
		// print: ¡®print¡¯ var_name¡¯;¡¯
		void expectStatementTypePrint();

		// Expects a call statement.
		// call: ¡®call¡¯ proc_name ¡®;¡¯
		void expectStatementTypeCall();

		// Expects a while container statement.
		// while: ¡®while¡¯ ¡®(¡¯ cond_expr ¡®)¡¯ ¡®{¡® stmtLst ¡®}¡¯
		void expectStatementTypeWhile();

		// Expects a if container statement.
		// if: ¡®if¡¯ ¡®(¡¯ cond_expr ¡®)¡¯ ¡®then¡¯ ¡®{¡® stmtLst ¡®}¡¯ ¡®else¡¯ ¡®{¡® stmtLst ¡®}¡¯
		void expectStatementTypeIf();

		// Expects a assignment statement.
		// assign: var_name ¡®=¡¯ expr ¡®;¡¯
		void expectStatementTypeAssign();

		// Expects a conditional expression.
		// cond_expr: rel_expr 
		//			| ¡®!¡¯ ¡®(¡¯ cond_expr ¡®)¡¯ 
		//			| ¡®(¡¯ cond_expr ¡®)¡¯ ¡®&&¡¯ ¡®(¡¯ cond_expr ¡®)¡¯ 
		//			| ¡®(¡¯ cond_expr ¡®)¡¯ ¡®||¡¯ ¡®(¡¯ cond_expr ¡®)¡¯
		void expectConditionalExpression();

		// Expects a relational expression.
		// rel_expr : rel_factor ¡®>¡¯ rel_factor
		//			| rel_factor ¡®>=¡¯ rel_factor 
		//			| rel_factor ¡®<¡¯ rel_factor 
		//			| rel_factor ¡®<=¡¯ rel_factor 
		//			| rel_factor ¡®==¡¯ rel_factor 
		//			| rel_factor ¡®!=¡¯ rel_factor
		void expectRelationalExpression();

		// Expects a replational factor.
		// rel_factor: var_name | const_value | expr
		void expectRelationalFactor();

		// Expects a expression.
		// expr: expr ¡®+¡¯ term | expr ¡®-¡¯ term | term
		void expectExpression();

		// Expects a term.
		// term: term ¡®*¡¯ factor | term ¡®/¡¯ factor | term ¡®%¡¯ factor | factor
		void expectTerm();

		// Expects a factor.
		// factor: var_name | const_value | ¡®(¡¯ expr ¡®)¡¯
		void expectFactor();

		// Expects an identifier.
		Token& expectIdentifier();

		// Optional relational expression.
		bool optionalRelationalExpression();

		// Optional replational factor.
		bool optionalRelationalFactor();

		// Optional expression.
		bool optionalExpression();

		// Optional term.
		bool optionalTerm();

		// Optional factor.
		bool optionalFactor();

		// Optional identifier.
		bool optionalIdentifier();

	private:
		Tokenizer tokenizer;
		Extractor *design_extractor;

		// Expects a compulsory token with given token type, or else the build fails.
		Token& expectTokenAndPop(TokenType);

		// This token can be optional, will return a boolean value indicating if that token exists
		bool peekToken(TokenType);

		// Expects an optional token with given token type.
		bool probeAndPop(TokenType);

		// Expects an optional token with given token type.
		bool probeAndPeek(TokenType);

		// Throws exception
		void unexpectedToken(std::string);
	};

} // namespace SourceProcessor
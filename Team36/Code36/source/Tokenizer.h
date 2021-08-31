#pragma once

#include <string>
#include <vector>
#include "Token.h"


namespace SourceProcessor {

	class Tokenizer {
	public:

		//Constructor
		Tokenizer();

		// General tokenizing. Parse input into a chain of tokens for further use.
		// Separator validations happens here.
		// IMPORTANT : due to the nature of fopen and fread, I am unsure about \n \r characters, this is only a temporary solution!
		void parse_into_tokens(const char* input);

		// Provide access to token_chain
		const std::vector<Token> get_token_chain() const;

		// Pseudo FSM to expect and accept tokens, interact PKB to build.
		//void build();

	private:
		std::vector<Token> m_token_cache;

		// Push token to token_cache
		void add_token(Token& token);



		// To simplify the parsing process, it is broken down into parsing procedure, stmts etc.
		// The following is defined according to the SIMPLE concrete syntax grammar definition order.

		/*
		* temporarily commented out as it will be shifted to another component.
		void expect_procedure();
		void expect_statement_list();
		void expect_statement();
		void expect_statement_type_read();
		void expect_statement_type_print();
		void expect_statement_type_call();
		void expect_statement_type_if();
		void expect_statement_type_while();
		void expect_statement_type_assign();
		void expect_conditional_expression();
		void expect_relational_expression();
		void expect_expression();
		void expect_relational_factor();
		void expect_term();
		void expect_factor();
		*/
	};

} // namespace SourceProcessor
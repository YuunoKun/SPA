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
		void parse_into_tokens(const char*);

		// Provide access to token_chain
		const std::vector<Token> &get_token_chain() const;

		// Init
		void init_token_stack();

		// Check if there is remaining tokens
		bool has_token();

		// Peek token.
		Token &peek_token();

		// Pop token
		Token &pop_token();

	private:
		std::vector<Token> m_token_cache;

		int m_pos;

		// Push token to token_cache
		void add_token(Token&);

	};

} // namespace SourceProcessor
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

		// Peek probe
		Token& peek_probe();

		// Pop probe
		Token& pop_probe();

		// Reset probe position to pos
		void reset_probe();

	private:
		std::vector<Token> m_token_cache;
		int m_pos;
		int m_probe;

		// Push token to token_cache
		void add_token(Token&);

	};

} // namespace SourceProcessor
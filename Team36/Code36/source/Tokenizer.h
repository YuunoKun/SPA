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
		void parseIntoTokens(const char*);

		// Make a copy
		std::vector<Token> getTokenChain();

		// Init
		void initTokenStack();

		// Check if there is remaining tokens
		bool hasToken();

		// Peek token.
		Token peekToken();

		// Pop token
		Token popToken();

		// Peek probe
		Token peekProbe();

		// Pop probe
		Token popProbe();

		// Reset probe position to pos
		void resetProbe();

	private:
		std::vector<Token> token_cache;
		size_t pos;
		size_t probe;

		// Push token to token_cache
		void addToken(Token&);

	};

} // namespace SourceProcessor
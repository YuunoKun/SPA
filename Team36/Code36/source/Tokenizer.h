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

		// Provide access to token_chain
		const std::vector<Token> &getTokenChain() const;

		// Init
		void initTokenStack();

		// Check if there is remaining tokens
		bool hasToken();

		// Peek token.
		Token &peekToken();

		// Pop token
		Token &popToken();

		// Peek probe
		Token& peekProbe();

		// Pop probe
		Token& popProbe();

		// Reset probe position to pos
		void resetProbe();

	private:
		std::vector<Token> token_cache;
		int pos;
		int probe;

		// Push token to token_cache
		void addToken(Token&);

	};

} // namespace SourceProcessor
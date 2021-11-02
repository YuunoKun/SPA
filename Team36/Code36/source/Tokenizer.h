#pragma once

#include <string>
#include <vector>
#include "SPCommon.h"
#include "Token.h"

namespace SourceProcessor {

	class Tokenizer {
	public:

		Tokenizer();

		std::vector<Token> getTokenChain();
		bool hasToken();
		Token peekToken();
		Token popToken();
		Token peekProbe();
		Token popProbe();
		void resetProbe();

		void parseIntoTokens(const char*);
		void initTokenStack();

	private:
		std::vector<Token> token_cache;
		size_t pos;
		size_t probe;

		void addToken(Token&);
	};

} // namespace SourceProcessor

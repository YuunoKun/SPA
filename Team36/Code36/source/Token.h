#pragma once

#include <string>
#include <vector>
#include "SPCommon.h"

namespace SourceProcessor {

	class Token {
	public:
		Token();
		Token(TokenType, std::string);

		TokenType getTokenType();
		std::string getTokenValue();
		void setTokenType(TokenType);
		void appendTokenValue(std::string);
		void appendTokenValue(char);
		void resetTokenValue();

	private:
		TokenType token_type;
		std::string token_value;
	};
	
} // namespace SourceProcessor

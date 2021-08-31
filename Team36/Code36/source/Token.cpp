#include <string>
#include "Token.h"

using namespace SourceProcessor;


Token::Token() {
	m_type = TokenType::WHITESPACE;
	m_token_value = "";
}


Token::Token(TokenType type, std::string token_value) {
	m_type = type;
	m_token_value = token_value;
}


std::string Token::to_string() {
	if (m_token_value == "") {
		return "Empty token.";
	}
	else {
		//return "Token type : " + tokenTypeStrings[m_type] + ", token value : " + m_token_value;
	}
}
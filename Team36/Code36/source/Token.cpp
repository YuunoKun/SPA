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


TokenType Token::get_token_type() {
	return m_type;
}


void Token::set_token_type(TokenType type) {
	m_type = type;
}


std::string &Token::get_token_value() {
	return m_token_value;
}


void Token::set_token_value(std::string value) {
	m_token_value = value;
}


std::string Token::to_string() {
	if (m_token_value == "") {
		return "Empty token.";
	}
	else {
		//return "Token type : " + tokenTypeStrings[m_type] + ", token value : " + m_token_value;
	}
}
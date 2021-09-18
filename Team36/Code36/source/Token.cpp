#include "Token.h"

using namespace SourceProcessor;


Token::Token() {
	token_type = TokenType::WHITESPACE;
	token_value = "";
}


Token::Token(TokenType type, std::string token_value) {
	token_type = type;
	token_value = token_value;
}


TokenType Token::getTokenType() {
	return token_type;
}


void Token::setTokenType(TokenType type) {
	token_type = type;
}


std::string &Token::getTokenValue() {
	return token_value;
}


void Token::setTokenValue(std::string value) {
	token_value = value;
}


std::string Token::toString() {
	if (token_value == "") {
		return "Empty token.";
	}
	else {
		//return "Token type : " + tokenTypeStrings[token_type] + ", token value : " + token_value;
	}
}
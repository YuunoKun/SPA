#include "Token.h"

using namespace SourceProcessor;


Token::Token() {
	token_type = TokenType::WHITESPACE;
	token_value = "";
}

Token::Token(TokenType type, std::string value) {
	token_type = type;
	token_value = value;
}

TokenType Token::getTokenType() {
	return token_type;
}

void Token::setTokenType(TokenType type) {
	token_type = type;
}

std::string Token::getTokenValue() {
	return token_value;
}

void Token::appendTokenValue(std::string value) {
	token_value += value;
}

void Token::appendTokenValue(char value) {
	token_value.push_back(value);
}

void Token::resetTokenValue() {
	token_value.clear();
}

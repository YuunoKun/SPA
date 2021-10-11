#include <stack>
#include <stdexcept>
#include <iostream>
#include "Tokenizer.h"

using namespace SourceProcessor;

Tokenizer::Tokenizer(void) {
	token_cache = {};
}

void Tokenizer::parseIntoTokens(const char* input) {
	token_cache = {};
	Token current_token;

	std::string source(input);
	for (char c : source) {
		//std::cout << (int)c << " ";
		switch (c) {
		case '{':
			addToken(current_token);
			current_token.setTokenType(TokenType::STATEMENT_LIST_OPEN);
			current_token.setTokenValue("{");
			addToken(current_token);
			break;
		case '}':
			addToken(current_token);
			current_token.setTokenType(TokenType::STATEMENT_LIST_CLOSE);
			current_token.setTokenValue("}");
			addToken(current_token);
			break;
		case '(':
			addToken(current_token);
			current_token.setTokenType(TokenType::PARENTHESIS_OPEN);
			current_token.setTokenValue("(");
			addToken(current_token);
			break;
		case ')':
			addToken(current_token);
			current_token.setTokenType(TokenType::PARENTHESIS_CLOSE);
			current_token.setTokenValue(")");
			addToken(current_token);
			break;
		case ';':
			addToken(current_token);
			current_token.setTokenType(TokenType::TERMINATOR);
			current_token.setTokenValue(";");
			addToken(current_token);
			break;
		case ' ':
		case '	':
			// Ignore extra white spaces, also terminates forming of potential identifiers
			// This also ensure that token values does not contain extra whitespaces.
			addToken(current_token);
			current_token.setTokenType(TokenType::WHITESPACE);
			break;
		case '&':
			if (current_token.getTokenType() == TokenType::BIT_AND) {
				current_token.setTokenType(TokenType::BOOL_AND);
				current_token.setTokenValue("&&");
				addToken(current_token);
			}
			else {
				addToken(current_token);
				current_token.setTokenType(TokenType::BIT_AND);
				current_token.setTokenValue("&");
			}
			break;
		case '|':
			if (current_token.getTokenType() == TokenType::BIT_OR) {
				current_token.setTokenType(TokenType::BOOL_OR);
				current_token.setTokenValue("||");
				addToken(current_token);
			}
			else {
				addToken(current_token);
				current_token.setTokenType(TokenType::BIT_OR);
				current_token.setTokenValue("|");
			}
			break;
		case '!':
			addToken(current_token);
			current_token.setTokenType(TokenType::BOOL_NEGATE);
			current_token.setTokenValue("!");
			break;
		case '>':
			addToken(current_token);
			current_token.setTokenType(TokenType::BOOL_GT);
			current_token.setTokenValue(">");
			break;
		case '<':
			addToken(current_token);
			current_token.setTokenType(TokenType::BOOL_LT);
			current_token.setTokenValue("<");
			break;
		case '=':
			if (current_token.getTokenType() == TokenType::BOOL_NEGATE) {
				current_token.setTokenType(TokenType::BOOL_NEQUIV);
				current_token.setTokenValue("!=");
			}
			else if (current_token.getTokenType() == TokenType::BOOL_LT) {
				current_token.setTokenType(TokenType::BOOL_LTEQ);
				current_token.setTokenValue("<=");
			}
			else if (current_token.getTokenType() == TokenType::BOOL_GT) {
				current_token.setTokenType(TokenType::BOOL_GTEQ);
				current_token.setTokenValue(">=");
			}
			else if (current_token.getTokenType() == TokenType::ASSIGN) {
				current_token.setTokenType(TokenType::BOOL_EQUIV);
				current_token.setTokenValue("==");
			}
			else {
				addToken(current_token);
				current_token.setTokenType(TokenType::ASSIGN);
				current_token.setTokenValue("=");
				break;
			}
			addToken(current_token);
			break;
		case '+':
			addToken(current_token);
			current_token.setTokenType(TokenType::PLUS);
			current_token.setTokenValue("+");
			addToken(current_token);
			break;
		case '-':
			addToken(current_token);
			current_token.setTokenType(TokenType::MINUS);
			current_token.setTokenValue("-");
			addToken(current_token);
			break;
		case '*':
			addToken(current_token);
			current_token.setTokenType(TokenType::MUL);
			current_token.setTokenValue("*");
			addToken(current_token);
			break;
		case '/':
			addToken(current_token);
			current_token.setTokenType(TokenType::DIV);
			current_token.setTokenValue("/");
			addToken(current_token);
			break;
		case '%':
			addToken(current_token);
			current_token.setTokenType(TokenType::MOD);
			current_token.setTokenValue("%");
			addToken(current_token);
			break;
		default:
			if (current_token.getTokenType() == TokenType::ASSIGN) {
				addToken(current_token);
			}

			if (isdigit(c)) {
				if (current_token.getTokenType() == TokenType::IDENTIFIER) {
					current_token.getTokenValue().push_back(c);
				}
				else if (current_token.getTokenType() == TokenType::WHITESPACE || current_token.getTokenType() == TokenType::CONSTANT) {
					current_token.setTokenType(TokenType::CONSTANT);
					current_token.getTokenValue().push_back(c);
				}
				else {
					addToken(current_token);
					current_token.setTokenType(TokenType::CONSTANT);
					current_token.getTokenValue().push_back(c);
				}
			}
			else if (isalpha(c)) {
				if (current_token.getTokenType() == TokenType::IDENTIFIER) {
					current_token.getTokenValue().push_back(c);
				}
				else {
					addToken(current_token);
					current_token.setTokenType(TokenType::IDENTIFIER);
					current_token.getTokenValue().push_back(c);
				}
			}
			else {
				throw std::runtime_error("Unknown symbol : \'" + c + '\'');
			}
			break;
		}
	}

	addToken(current_token);
}


std::vector<Token> Tokenizer::getTokenChain() {
	return token_cache;
}


void Tokenizer::addToken(Token& token) {

	if (token.getTokenType() == TokenType::WHITESPACE) {
		//discard whitespace
		return;
	}

	if (token.getTokenType() == TokenType::IDENTIFIER) {
		if (token.getTokenValue() == "procedure") {
			token.setTokenType(TokenType::PROCEDURE);
		}
		else if (token.getTokenValue() == "read") {
			token.setTokenType(TokenType::READ);
		}
		else if (token.getTokenValue() == "print") {
			token.setTokenType(TokenType::PRINT);
		}
		else if (token.getTokenValue() == "call") {
			token.setTokenType(TokenType::CALL);
		}
		else if (token.getTokenValue() == "if") {
			token.setTokenType(TokenType::IF);
		}
		else if (token.getTokenValue() == "while") {
			token.setTokenType(TokenType::WHILE);
		}
		else if (token.getTokenValue() == "then") {
			token.setTokenType(TokenType::THEN);
		}
		else if (token.getTokenValue() == "else") {
			token.setTokenType(TokenType::ELSE);
		}
	}

	token_cache.push_back(token);
	token.setTokenType(TokenType::WHITESPACE);
	token.setTokenValue("");
}


void Tokenizer::initTokenStack() {
	pos = 0;
	probe = 0;
}


bool Tokenizer::hasToken() {
	return pos < token_cache.size();
}


Token Tokenizer::peekToken() {
	if (!hasToken()) {
		return Token(TokenType::INVAL, "");
	}
	else {
		probe = pos;
		return token_cache[pos];
	}
}


Token Tokenizer::popToken() {
	if (!hasToken()) {
		return Token(TokenType::INVAL, "");
	}
	else {
		probe = pos + 1;
		return token_cache[pos++];
	}
}


Token Tokenizer::peekProbe() {
	if (probe >= token_cache.size()) {
		return Token(TokenType::INVAL, "");
	}
	else {
		return token_cache[probe];
	}
}


Token Tokenizer::popProbe() {
	if (probe >= token_cache.size()) {
		return Token(TokenType::INVAL, "");
	}
	else {
		return token_cache[probe++];
	}
}


void Tokenizer::resetProbe() {
	probe = pos;
}

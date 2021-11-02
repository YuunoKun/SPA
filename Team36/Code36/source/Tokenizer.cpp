#include <stack>
#include <stdexcept>
#include <iostream>
#include "Tokenizer.h"

using namespace SourceProcessor;

Tokenizer::Tokenizer() {
	token_cache = {};
}

void Tokenizer::parseIntoTokens(const char* input) {
	token_cache = {};
	Token current_token;

	std::string source(input);
	for (char c : source) {
		switch (c) {
			case SYMBOL_LEFT_BRACE:
				addToken(current_token);
				current_token.setTokenType(TokenType::STATEMENT_LIST_OPEN);
				current_token.appendTokenValue(SYMBOL_LEFT_BRACE);
				addToken(current_token);
				break;
			case SYMBOL_RIGHT_BRACE:
				addToken(current_token);
				current_token.setTokenType(TokenType::STATEMENT_LIST_CLOSE);
				current_token.appendTokenValue(SYMBOL_RIGHT_BRACE);
				addToken(current_token);
				break;
			case SYMBOL_LEFT_PARENTHESIS:
				addToken(current_token);
				current_token.setTokenType(TokenType::PARENTHESIS_OPEN);
				current_token.appendTokenValue(SYMBOL_LEFT_PARENTHESIS);
				addToken(current_token);
				break;
			case SYMBOL_RIGHT_PARENTHESIS:
				addToken(current_token);
				current_token.setTokenType(TokenType::PARENTHESIS_CLOSE);
				current_token.appendTokenValue(SYMBOL_RIGHT_PARENTHESIS);
				addToken(current_token);
				break;
			case SYMBOL_SEMICOLON:
				addToken(current_token);
				current_token.setTokenType(TokenType::TERMINATOR);
				current_token.appendTokenValue(SYMBOL_SEMICOLON);
				addToken(current_token);
				break;
			case SYMBOL_SPACE:
			case SYMBOL_TAB:
				addToken(current_token);
				current_token.setTokenType(TokenType::WHITESPACE);
				break;
			case SYMBOL_AMPERSAND:
				if (current_token.getTokenType() == TokenType::BIT_AND) {
					current_token.setTokenType(TokenType::BOOL_AND);
					current_token.appendTokenValue(SYMBOL_AMPERSAND);
					addToken(current_token);
				}
				else {
					addToken(current_token);
					current_token.setTokenType(TokenType::BIT_AND);
					current_token.appendTokenValue(SYMBOL_AMPERSAND);
				}
				break;
			case SYMBOL_VERTICAL_BAR:
				if (current_token.getTokenType() == TokenType::BIT_OR) {
					current_token.setTokenType(TokenType::BOOL_OR);
					current_token.appendTokenValue(SYMBOL_VERTICAL_BAR);
					addToken(current_token);
				}
				else {
					addToken(current_token);
					current_token.setTokenType(TokenType::BIT_OR);
					current_token.appendTokenValue(SYMBOL_VERTICAL_BAR);
				}
				break;
			case SYMBOL_EXCLAMATION_MARK:
				addToken(current_token);
				current_token.setTokenType(TokenType::BOOL_NEGATE);
				current_token.appendTokenValue(SYMBOL_EXCLAMATION_MARK);
				break;
			case SYMBOL_GREATER_THAN_SIGN:
				addToken(current_token);
				current_token.setTokenType(TokenType::BOOL_GT);
				current_token.appendTokenValue(SYMBOL_GREATER_THAN_SIGN);
				break;
			case SYMBOL_LESS_THAN_SIGN:
				addToken(current_token);
				current_token.setTokenType(TokenType::BOOL_LT);
				current_token.appendTokenValue(SYMBOL_LESS_THAN_SIGN);
				break;
			case SYMBOL_EQUAL_SIGN:
				if (current_token.getTokenType() == TokenType::BOOL_NEGATE) {
					current_token.setTokenType(TokenType::BOOL_NEQUIV);
				}
				else if (current_token.getTokenType() == TokenType::BOOL_LT) {
					current_token.setTokenType(TokenType::BOOL_LTEQ);
				}
				else if (current_token.getTokenType() == TokenType::BOOL_GT) {
					current_token.setTokenType(TokenType::BOOL_GTEQ);
				}
				else if (current_token.getTokenType() == TokenType::ASSIGN) {
					current_token.setTokenType(TokenType::BOOL_EQUIV);
				}
				else {
					addToken(current_token);
					current_token.setTokenType(TokenType::ASSIGN);
					current_token.appendTokenValue(SYMBOL_EQUAL_SIGN);
					break;
				}
				current_token.appendTokenValue(SYMBOL_EQUAL_SIGN);
				addToken(current_token);
				break;
			case SYMBOL_PLUS_SIGN:
				addToken(current_token);
				current_token.setTokenType(TokenType::PLUS);
				current_token.appendTokenValue(SYMBOL_PLUS_SIGN);
				addToken(current_token);
				break;
			case SYMBOL_MINUS_SIGN:
				addToken(current_token);
				current_token.setTokenType(TokenType::MINUS);
				current_token.appendTokenValue(SYMBOL_MINUS_SIGN);
				addToken(current_token);
				break;
			case SYMBOL_ASTERISK:
				addToken(current_token);
				current_token.setTokenType(TokenType::MUL);
				current_token.appendTokenValue(SYMBOL_ASTERISK);
				addToken(current_token);
				break;
			case SYMBOL_SLASH:
				addToken(current_token);
				current_token.setTokenType(TokenType::DIV);
				current_token.appendTokenValue(SYMBOL_SLASH);
				addToken(current_token);
				break;
			case SYMBOL_PERCENT_SIGN:
				addToken(current_token);
				current_token.setTokenType(TokenType::MOD);
				current_token.appendTokenValue(SYMBOL_PERCENT_SIGN);
				addToken(current_token);
				break;
			default:
				if (current_token.getTokenType() == TokenType::ASSIGN) {
					addToken(current_token);
				}

				if (isdigit(c)) {
					if (current_token.getTokenType() == TokenType::IDENTIFIER) {
						current_token.appendTokenValue(c);
					}
					else if (current_token.getTokenType() == TokenType::WHITESPACE || current_token.getTokenType() == TokenType::CONSTANT) {
						current_token.setTokenType(TokenType::CONSTANT);
						current_token.appendTokenValue(c);
					}
					else {
						addToken(current_token);
						current_token.setTokenType(TokenType::CONSTANT);
						current_token.appendTokenValue(c);
					}
				}
				else if (isalpha(c)) {
					if (current_token.getTokenType() == TokenType::IDENTIFIER) {
						current_token.appendTokenValue(c);
					}
					else {
						addToken(current_token);
						current_token.setTokenType(TokenType::IDENTIFIER);
						current_token.appendTokenValue(c);
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
		return;
	}

	if (token.getTokenType() == TokenType::IDENTIFIER) {
		if (token.getTokenValue() == KEYWORD_PROCEDURE) {
			token.setTokenType(TokenType::PROCEDURE);
		}
		else if (token.getTokenValue() == KEYWORD_READ) {
			token.setTokenType(TokenType::READ);
		}
		else if (token.getTokenValue() == KEYWORD_PRINT) {
			token.setTokenType(TokenType::PRINT);
		}
		else if (token.getTokenValue() == KEYWORD_CALL) {
			token.setTokenType(TokenType::CALL);
		}
		else if (token.getTokenValue() == KEYWORD_IF) {
			token.setTokenType(TokenType::IF);
		}
		else if (token.getTokenValue() == KEYWORD_WHILE) {
			token.setTokenType(TokenType::WHILE);
		}
		else if (token.getTokenValue() == KEYWORD_THEN) {
			token.setTokenType(TokenType::THEN);
		}
		else if (token.getTokenValue() == KEYWORD_ELSE) {
			token.setTokenType(TokenType::ELSE);
		}
	}

	token_cache.push_back(token);
	token.setTokenType(TokenType::WHITESPACE);
	token.resetTokenValue();
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
		return Token(TokenType::INVAL, TOKENVALUE_PLACEHOLDER);
	}
	else {
		probe = pos;
		return token_cache[pos];
	}
}

Token Tokenizer::popToken() {
	if (!hasToken()) {
		return Token(TokenType::INVAL, TOKENVALUE_PLACEHOLDER);
	}
	else {
		probe = pos + 1;
		return token_cache[pos++];
	}
}

Token Tokenizer::peekProbe() {
	if (probe >= token_cache.size()) {
		return Token(TokenType::INVAL, TOKENVALUE_PLACEHOLDER);
	}
	else {
		return token_cache[probe];
	}
}

Token Tokenizer::popProbe() {
	if (probe >= token_cache.size()) {
		return Token(TokenType::INVAL, TOKENVALUE_PLACEHOLDER);
	}
	else {
		return token_cache[probe++];
	}
}

void Tokenizer::resetProbe() {
	probe = pos;
}

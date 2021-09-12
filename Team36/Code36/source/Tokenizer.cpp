#include <stack>
#include <stdexcept>
#include <iostream>
#include "Tokenizer.h"

using namespace SourceProcessor;

Tokenizer::Tokenizer(void) {
	m_token_cache = {};
}

void Tokenizer::parse_into_tokens(const char* input) {
	m_token_cache = {};
	Token current_token;
	std::stack<char> separator_validation_stk;
	std::string source = std::string(input);
	for (char c : source) {
		//std::cout << (int)c << " ";
		switch (c) {
		case '{':
			add_token(current_token);
			current_token.set_token_type(TokenType::STATEMENT_LIST_OPEN);
			add_token(current_token);
			separator_validation_stk.push('{');
			break;
		case '}':
			if (!separator_validation_stk.empty() && separator_validation_stk.top() == '{') {
				separator_validation_stk.pop();
			}
			else {
				//throw std::runtime_error("Unexpected symbol : \'" + c + '\'');
			}
			add_token(current_token);
			current_token.set_token_type(TokenType::STATEMENT_LIST_CLOSE);
			add_token(current_token);
			break;
		case '(':
			add_token(current_token);
			current_token.set_token_type(TokenType::PARENTHESIS_OPEN);
			add_token(current_token);
			separator_validation_stk.push('(');
			break;
		case ')':
			if (!separator_validation_stk.empty() && separator_validation_stk.top() == '(') {
				separator_validation_stk.pop();
			}
			else {
				//throw std::runtime_error("Unexpected symbol : \'" + c + '\'');
			}
			add_token(current_token);
			current_token.set_token_type(TokenType::PARENTHESIS_CLOSE);
			add_token(current_token);
			break;
		case ';':
			add_token(current_token);
			current_token.set_token_type(TokenType::TERMINATOR);
			add_token(current_token);
			break;
		case ' ':
			// Ignore extra white spaces, also terminates forming of potential identifiers
			// This also ensure that token values does not contain extra whitespaces.
			add_token(current_token);
			current_token.set_token_type(TokenType::WHITESPACE);
			break;
		case '&':
			if (current_token.get_token_type() == TokenType::BIT_AND) {
				current_token.set_token_type(TokenType::BOOL_AND);
				add_token(current_token);
			}
			else {
				add_token(current_token);
				current_token.set_token_type(TokenType::BIT_AND);
			}
			break;
		case '|':
			if (current_token.get_token_type() == TokenType::BIT_OR) {
				current_token.set_token_type(TokenType::BOOL_OR);
				add_token(current_token);
			}
			else {
				add_token(current_token);
				current_token.set_token_type(TokenType::BIT_OR);
			}
			break;
		case '!':
			add_token(current_token);
			current_token.set_token_type(TokenType::BOOL_NEGATE);
			break;
		case '>':
			add_token(current_token);
			current_token.set_token_type(TokenType::BOOL_GT);
			break;
		case '<':
			add_token(current_token);
			current_token.set_token_type(TokenType::BOOL_LT);
			break;
		case '=':
			if (current_token.get_token_type() == TokenType::BOOL_NEGATE) {
				current_token.set_token_type(TokenType::BOOL_NEQUIV);
			}
			else if (current_token.get_token_type() == TokenType::BOOL_LT) {
				current_token.set_token_type(TokenType::BOOL_LTEQ);
			}
			else if (current_token.get_token_type() == TokenType::BOOL_GT) {
				current_token.set_token_type(TokenType::BOOL_GTEQ);
			}
			else if (current_token.get_token_type() == TokenType::ASSIGN) {
				current_token.set_token_type(TokenType::BOOL_EQUIV);
			}
			else {
				add_token(current_token);
				current_token.set_token_type(TokenType::ASSIGN);
				break;
			}
			add_token(current_token);
			break;
		case '+':
			add_token(current_token);
			current_token.set_token_type(TokenType::PLUS);
			add_token(current_token);
			break;
		case '-':
			add_token(current_token);
			current_token.set_token_type(TokenType::MINUS);
			add_token(current_token);
			break;
		case '*':
			add_token(current_token);
			current_token.set_token_type(TokenType::MUL);
			add_token(current_token);
			break;
		case '/':
			add_token(current_token);
			current_token.set_token_type(TokenType::DIV);
			add_token(current_token);
			break;
		case '%':
			add_token(current_token);
			current_token.set_token_type(TokenType::MOD);
			add_token(current_token);
			break;
		default:
			if (current_token.get_token_type() == TokenType::ASSIGN) {
				add_token(current_token);
			}

			if (isdigit(c)) {
				if (current_token.get_token_type() == TokenType::IDENTIFIER) {
					current_token.get_token_value().push_back(c);
				}
				else if (current_token.get_token_type() == TokenType::WHITESPACE || current_token.get_token_type() == TokenType::CONSTANT) {
					current_token.set_token_type(TokenType::CONSTANT);
					current_token.get_token_value().push_back(c);
				}
				else {
					add_token(current_token);
					current_token.set_token_type(TokenType::CONSTANT);
					current_token.get_token_value().push_back(c);
				}
			}
			else if (isalpha(c)) {
				if (current_token.get_token_type() == TokenType::IDENTIFIER) {
					current_token.get_token_value().push_back(c);
				}
				else {
					add_token(current_token);
					current_token.set_token_type(TokenType::IDENTIFIER);
					current_token.get_token_value().push_back(c);
				}
			}
			else {
				throw std::runtime_error("Unknown symbol : \'" + c + '\'');
			}
			break;
		}
	}

	add_token(current_token);
}


const std::vector<Token> &Tokenizer::get_token_chain() const {
	return m_token_cache;
}


void Tokenizer::add_token(Token& token) {

	if (token.get_token_type() == TokenType::WHITESPACE) {
		//discard whitespace
		return;
	}

	if (token.get_token_type() == TokenType::IDENTIFIER) {
		if (token.get_token_value() == "procedure") {
			token.set_token_type(TokenType::PROCEDURE);
		}
		else if (token.get_token_value() == "read") {
			token.set_token_type(TokenType::READ);
		}
		else if (token.get_token_value() == "print") {
			token.set_token_type(TokenType::PRINT);
		}
		else if (token.get_token_value() == "call") {
			token.set_token_type(TokenType::CALL);
		}
		else if (token.get_token_value() == "if") {
			token.set_token_type(TokenType::IF);
		}
		else if (token.get_token_value() == "while") {
			token.set_token_type(TokenType::WHILE);
		}
		else if (token.get_token_value() == "then") {
			token.set_token_type(TokenType::THEN);
		}
		else if (token.get_token_value() == "else") {
			token.set_token_type(TokenType::ELSE);
		}
	}

	m_token_cache.push_back(token);
	token.set_token_type(TokenType::WHITESPACE);
	token.set_token_value("");
}


void Tokenizer::init_token_stack() {
	m_pos = 0;
}


bool Tokenizer::has_token() {
	return m_pos < m_token_cache.size();
}


Token &Tokenizer::peek_token() {
	if (!has_token()) {
		return Token(TokenType::INVAL, "");
	}
	else {
		return m_token_cache[m_pos];
	}
}


Token &Tokenizer::pop_token() {
	if (!has_token()) {
		return Token(TokenType::INVAL, "");
	}
	else {
		return m_token_cache[m_pos++];
	}
}

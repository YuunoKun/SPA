#include <string>
#include <vector>
#include <stack>
#include <stdexcept>
#include "Token.h"
#include "Tokenizer.h"

using namespace Tokenizer;

Tokenizer::Tokenizer() {
	m_token_cache = {};
}

void Tokenizer::parse_into_tokens(const char* input) {
	Token current_token;
	std::stack<char> separator_validation_stk;

	for (char c : input) {
		switch (c) {
		case '{':
			add_token(current_token);
			current_token.m_type = TokenType::STATEMENT_LIST_OPEN;
			add_token(current_token);
			separator_validation_stk.push('{');
			break;
		case '}':
			if (!separator_validation_stk.empty() && separator_validation_stk.top() == '{') {
				separator_validation_stk.pop();
			}
			else {
				std::runtime_error("Unexpected symbol : \'" + c + "\'");
			}
			add_token(current_token);
			current_token.m_type = TokenType::STATEMENT_LIST_CLOSE;
			add_token(current_token);
			break;
		case '(':
			add_token(current_token);
			current_token.m_type = TokenType::PARENTHESIS_OPEN;
			add_token(current_token);
			separator_validation_stk.push('(');
			break;
		case ')':
			if (!separator_validation_stk.empty() && separator_validation_stk.top() == '(') {
				separator_validation_stk.pop();
			}
			else {
				std::runtime_error("Unexpected symbol : \'" + c + "\'");
			}
			add_token(current_token);
			current_token.m_type = TokenType::PARENTHESIS_CLOSE;
			add_token(current_token);
			break;
		case ';':
			add_token(current_token);
			current_token.m_type = TokenType::TERMINATOR;
			add_token(current_token);
			break;
		case ' ':
			// Ignore extra white spaces, also terminates forming of potential identifiers
			// This also ensure that token values does not contain extra whitespaces.
			add_token(current_token);
			current_token.m_type = TokenType::WHITESPACE;
			break;
		case '&':
			if (current_token.m_type == TokenType::BIT_AND) {
				current_token.m_type = TokenType::BOOL_AND;
				add_token(current_token);
			}
			else {
				add_token(current_token);
				current_token.m_type = TokenType::BIT_AND;
			}
			break;
		case '|':
			if (current_token.m_type == TokenType::BIT_OR) {
				current_token.m_type = TokenType::BOOL_OR;
				add_token(current_token);
			}
			else {
				add_token(current_token);
				current_token.m_type = TokenType::BIT_OR;
			}
			break;
		case '!':
			add_token(current_token);
			current_token.m_type = TokenType::BOOL_NEGATE;
			break;
		case '>':
			add_token(current_token);
			current_token.m_type = TokenType::BOOL_GT;
			break;
		case '<':
			add_token(current_token);
			current_token.m_type = TokenType::BOOL_LT;
			break;
		case '=':
			if (current_token.m_type == TokenType::BOOL_NEGATE) {
				current_token.m_type = TokenType::BOOL_NEQUIV;
				add_token(current_token);
			}
			else if (current_token.m_type == TokenType::BOOL_LT) {
				current_token.m_type = TokenType::BOOL_LTEQ;
				add_token(current_token);
			}
			else if (current_token.m_type == TokenType::BOOL_GT) {
				current_token.m_type = TokenType::BOOL_GTEQ;
				add_token(current_token);
			}
			else if (current_token.m_type == TokenType::ASSIGN) {
				current_token.m_type = TokenType::BOOL_EQUIV;
				add_token(current_token);
			}
			else {
				current_token.m_type = TokenType::ASSIGN;
			}
			break;
		case '+':
			add_token(current_token);
			current_token.m_type = TokenType::PLUS;
			add_token(current_token);
			break;
		case '-':
			add_token(current_token);
			current_token.m_type = TokenType::MINUS;
			add_token(current_token);
			break;
		case '*':
			add_token(current_token);
			current_token.m_type = TokenType::MUL;
			add_token(current_token);
			break;
		case '/':
			add_token(current_token);
			current_token.m_type = TokenType::DIV;
			add_token(current_token);
			break;
		case '%':
			add_token(current_token);
			current_token.m_type = TokenType::MOD;
			add_token(current_token);
			break;
		default:
			if (isdigit(c)) {
				if (current_token.m_type == TokenType::IDENTIFIER) {
					current_token.m_token_value.push_back(c);
				}
				else if (current_token.m_type == TokenType::WHITESPACE || current_token.m_type == TokenType::CONSTANT) {
					current_token.m_type = TokenType::CONSTANT;
					current_token.m_token_value.push_back(c);
				}
			}
			else if (isalpha(c)) {
				current_token.m_type = TokenType::INDENTIFIER;
				current_token.m_token_value.push_back(c);
			}
			else {
				std::runtime_error("Unknown symbol : \'" + c + "\'");
			}
			break;
		}
	}
}

const std::vector<Token> &Tokenizer::get_token_chain() const {
	return m_token_chain;
}

void Tokenizer::add_token(Token& token) {

	if (token.m_type == TokenType::WHITESPACE) {
		//discard whitespace
		return;
	}

	if (token.m_type == TokenType::IDENTIFIER) {
		switch (token.m_token_value) {
		case "procedure":
			token.m_type = TokenType::PROCEDURE;
			break;
		case "read":
			token.m_type = TokenType::READ;
			break;
		case "print":
			token.m_type = TokenType::PRINT;
			break;
		case "call":
			token.m_type = TokenType::CALL;
			break;
		case "if":
			token.m_type = TokenType::IF;
			break;
		case "while":
			token.m_type = TokenType::WHILE;
			break;
		case "then":
			token.m_type = TokenType::THEN;
			break;
		case "else":
			token.m_type = TokenType::ELSE;
			break;
		default:
			break;
		}
	}

	m_token_cache.push_back(token);
	token.m_type = TokenType::WHITESPACE;
	token.m_token_value = "";
}

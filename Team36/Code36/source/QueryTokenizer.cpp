#include <string>
#include <vector>
#include <stdexcept>
#include "QueryTokenizer.h"

QueryTokenizer::QueryTokenizer() {
	query_token_cache = {};
}

void QueryTokenizer::parse_into_query_tokens(std::string input) {
	QueryToken curr_query_token;

	for (char c : input) {
		switch (c) {
		case ' ':
			add_query_token(curr_query_token);
			curr_query_token.type = QueryToken::QueryTokenType::WHITESPACE;
			break;
		default:
			if (isdigit(c)) {
				if (curr_query_token.type == QueryToken::QueryTokenType::IDENTIFIER) {
					curr_query_token.token_value.push_back(c);
				}
				else if (curr_query_token.type == QueryToken::QueryTokenType::WHITESPACE || curr_query_token.type == QueryToken::QueryTokenType::CONSTANT) {
					curr_query_token.type = QueryToken::QueryTokenType::CONSTANT;
					curr_query_token.token_value.push_back(c);
				}
			}
			else if (isalpha(c)) {
				curr_query_token.type = QueryToken::QueryTokenType::IDENTIFIER;
				curr_query_token.token_value.push_back(c);
			}
			else {
				std::runtime_error("Unknown symbol : \'" + c + '\'');
			}
			break;
		}
	}
}

std::vector<QueryToken> QueryTokenizer::get_query_token_chain() {
	return query_token_cache;
}

void QueryTokenizer::add_query_token(QueryToken& query_token) {
	if (query_token.type == QueryToken::WHITESPACE) {
		//discard whitespace
		return;
	}

	// send it as WORD/identifier to query_token_chain

	/*if (token.m_type == TokenType::IDENTIFIER) {
		if (token.m_token_value == "procedure") {
			token.m_type = TokenType::PROCEDURE;
		}
		else if (token.m_token_value == "read") {
			token.m_type = TokenType::READ;
		}
		else if (token.m_token_value == "print") {
			token.m_type = TokenType::PRINT;
		}
		else if (token.m_token_value == "call") {
			token.m_type = TokenType::CALL;
		}
		else if (token.m_token_value == "if") {
			token.m_type = TokenType::IF;
		}
		else if (token.m_token_value == "while") {
			token.m_type = TokenType::WHILE;
		}
		else if (token.m_token_value == "then") {
			token.m_type = TokenType::THEN;
		}
		else if (token.m_token_value == "else") {
			token.m_type = TokenType::ELSE;
		}
	}
	*/

	query_token_cache.push_back(query_token);
	query_token.type = QueryToken::QueryTokenType::WHITESPACE;
	query_token.token_value = "";
}
#include "QueryPreprocessor.h"
#include <iostream>
#include <string>
#include <vector>
#include "QueryToken.h"
#include "QueryTokenizer.h"
#include "Query.h"

QueryPreprocessor::QueryPreprocessor() {
}

Query QueryPreprocessor::parse(std::string str) {
	Query query = Query();
	QueryTokenizer query_tokenizer = QueryTokenizer();

	//query_tokenizer.parse_into_query_tokens(str);

	//const std::vector<Token> v = query_tokenizer.get_query_token_chain();

	std::vector<QueryToken> v;
	v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "stmt" });
	v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "s" });
	v.push_back({ QueryToken::QueryTokenType::COMMA, "," });
	v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "ss" });
	v.push_back({ QueryToken::QueryTokenType::TERMINATOR, ";" });
	v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "procedure" });
	v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "p" });
	v.push_back({ QueryToken::QueryTokenType::TERMINATOR, ";" });
	v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "Select" });
	v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "s" });

	v = QueryPreprocessor::modify_token_types(v);
	return query;
}

std::vector<QueryToken> QueryPreprocessor::modify_token_types(std::vector<QueryToken> input) {
	std::vector<QueryToken> output;
	QueryToken prevToken = QueryToken();

	for (QueryToken token : input) {
		QueryToken temp;
		// First iteration, set identifier to correct type
		if (prevToken.type == QueryToken::QueryTokenType::WHITESPACE) {
			if (token.token_value == "stmt") {
				temp = { QueryToken::QueryTokenType::STMT, "stmt" };
				output.push_back(temp);
			}
			else if (token.token_value == "procedure") {
				temp = { QueryToken::QueryTokenType::PROCEDURE, "procedure" };
				output.push_back(temp);
			}
			else if (token.token_value == "read") {
				temp = { QueryToken::QueryTokenType::READ, "read" };
				output.push_back(temp);
			}
			else if (token.token_value == "print") {
				temp = { QueryToken::QueryTokenType::PRINT, "print" };
				output.push_back(temp);
			}
			else if (token.token_value == "call") {
				temp = { QueryToken::QueryTokenType::CALL, "call" };
				output.push_back(temp);
			}
			else if (token.token_value == "if") {
				temp = { QueryToken::QueryTokenType::IF, "if" };
				output.push_back(temp);
			}
			else if (token.token_value == "while") {
				temp = { QueryToken::QueryTokenType::WHILE, "while" };
				output.push_back(temp);
			}
			else if (token.token_value == "assign") {
				temp = { QueryToken::QueryTokenType::ASSIGN, "assign" };
				output.push_back(temp);
			}
			else if (token.token_value == "variable") {
				temp = { QueryToken::QueryTokenType::VARIABLE, "variable" };
				output.push_back(temp);
			}
			else if (token.token_value == "constant") {
				temp = { QueryToken::QueryTokenType::CONSTANT, "constant" };
				output.push_back(temp);
			}
		}
		// Not first iteration, prevToken not NULL
		else {
		}
		prevToken = temp;
	}
	return output;
}
#include "QueryPreprocessor.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "QueryToken.h"
#include "QueryTokenizer.h"
#include "Query.h"

QueryPreprocessor::QueryPreprocessor() {
}

Query QueryPreprocessor::parse(std::string str) {
	Query query;
	QueryTokenizer query_tokenizer;

	//query_tokenizer.parse_into_query_tokens(str);

	//const std::vector<Token> v = query_tokenizer.get_query_token_chain();

	std::vector<QueryToken> v;
	QueryToken q1(QueryToken::QueryTokenType::IDENTIFIER, "stmt");
	v.push_back(q1);
	v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "s" });
	v.push_back({ QueryToken::QueryTokenType::COMMA, "," });
	v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "ss" });
	v.push_back({ QueryToken::QueryTokenType::TERMINATOR, ";" });
	v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "procedure" });
	v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "p" });
	v.push_back({ QueryToken::QueryTokenType::TERMINATOR, ";" });
	v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "Select" });
	v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "s" });

	QueryToken prevToken = QueryToken();
	bool isDeclare = true;
	std::vector<QueryToken> output;

	for (QueryToken token : v) {
		QueryToken temp;

		if (token.type == QueryToken::QueryTokenType::SELECT) {
			isDeclare = false;
		}

		// First iteration, set identifier to correct type
		if (isDeclare = true) {
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

			// Guard clauses to catch semantically wrong input
			if (prevToken.type == QueryToken::QueryTokenType::IDENTIFIER &&
				(token.type != QueryToken::QueryTokenType::COMMA &&
					token.type != QueryToken::QueryTokenType::TERMINATOR)) {
				throw std::runtime_error("During declaration, only comma and terminator is accepted after identifier.");
			}
			if (prevToken.type == QueryToken::QueryTokenType::COMMA &&
				token.type != QueryToken::QueryTokenType::IDENTIFIER) {
				throw std::runtime_error("During declaration, only identifier is accepted after comma.");
			}
			// No change if previous token is declared types
			if (prevToken.type == QueryToken::QueryTokenType::STMT ||
				prevToken.type == QueryToken::QueryTokenType::READ ||
				prevToken.type == QueryToken::QueryTokenType::PRINT ||
				prevToken.type == QueryToken::QueryTokenType::CALL ||
				prevToken.type == QueryToken::QueryTokenType::IF ||
				prevToken.type == QueryToken::QueryTokenType::WHILE ||
				prevToken.type == QueryToken::QueryTokenType::ASSIGN ||
				prevToken.type == QueryToken::QueryTokenType::VARIABLE ||
				prevToken.type == QueryToken::QueryTokenType::CONSTANT ||
				prevToken.type == QueryToken::QueryTokenType::PROCEDURE ||
				prevToken.type == QueryToken::QueryTokenType::COMMA) {
				temp = token;
				output.push_back(temp);
				// TODO: Add entity instead of output here
			} 
			else if (token.type == QueryToken::QueryTokenType::COMMA && token.token_value == "," ||
				token.type == QueryToken::QueryTokenType::COMMA && token.token_value == "," ||
				token.type == QueryToken::QueryTokenType::TERMINATOR && token.token_value == ";") {
				temp = token;
			} 
		}
		// No more declaration, Select statement as isDeclare is false
		else {
			if (prevToken.type == QueryToken::QueryTokenType::SELECT &&
				token.type != QueryToken::QueryTokenType::TERMINATOR) {
				throw std::runtime_error("Select must come after terminator.");
			}
			if (prevToken.type == QueryToken::QueryTokenType::SELECT) {
				temp = token;
				output.push_back(temp);
			}
			if (token.token_value == "Select") {
				temp = { QueryToken::QueryTokenType::SELECT, "Select" };
				output.push_back(temp);
			}
		}
		
		prevToken = temp;
	}
	return query;
}
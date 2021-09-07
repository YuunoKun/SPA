#include "QueryPreprocessor.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "Entity.h"
#include "QueryToken.h"
#include "QueryTokenizer.h"
#include "Query.h"

QueryPreprocessor::QueryPreprocessor() {
}

Query QueryPreprocessor::parse(std::string str) {
	Query query = Query();
	QueryTokenizer query_tokenizer;

	query_tokenizer.parse_into_query_tokens(str);

	const std::vector<QueryToken> v = query_tokenizer.get_query_token_chain();

	//std::vector<QueryToken> v;
	//QueryToken q1(QueryToken::QueryTokenType::IDENTIFIER, "stmt");
	//v.push_back(q1);
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "s" });
	//v.push_back({ QueryToken::QueryTokenType::COMMA, "," });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "ss" });
	//v.push_back({ QueryToken::QueryTokenType::TERMINATOR, ";" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "procedure" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "p" });
	//v.push_back({ QueryToken::QueryTokenType::TERMINATOR, ";" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "Select" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "s" });
	//v.push_back({ QueryToken::QueryTokenType::COMMA, "," });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "p" });

	QueryToken prevToken = QueryToken();
	std::vector<QueryToken> output;
	std::vector<QueryToken> selected;
	QueryToken temp;

	for (QueryToken token : v) {
		// First iteration, set identifier to correct type
		// Check what is my previous token
		if (prevToken.type == QueryToken::QueryTokenType::WHITESPACE ||
			prevToken.type == QueryToken::QueryTokenType::TERMINATOR) {
			if (token.token_value == "stmt") {
				temp = { QueryToken::QueryTokenType::STMT, "stmt" };
			}
			else if (token.token_value == "procedure") {
				temp = { QueryToken::QueryTokenType::PROCEDURE, "procedure" };
			}
			else if (token.token_value == "read") {
				temp = { QueryToken::QueryTokenType::READ, "read" };
			}
			else if (token.token_value == "print") {
				temp = { QueryToken::QueryTokenType::PRINT, "print" };
			}
			else if (token.token_value == "call") {
				temp = { QueryToken::QueryTokenType::CALL, "call" };
			}
			else if (token.token_value == "if") {
				temp = { QueryToken::QueryTokenType::IF, "if" };
			}
			else if (token.token_value == "while") {
				temp = { QueryToken::QueryTokenType::WHILE, "while" };
			}
			else if (token.token_value == "assign") {
				temp = { QueryToken::QueryTokenType::ASSIGN, "assign" };
			}
			else if (token.token_value == "variable") {
				temp = { QueryToken::QueryTokenType::VARIABLE, "variable" };
			}
			else if (token.token_value == "constant") {
				temp = { QueryToken::QueryTokenType::CONSTANT, "constant" };
			}
			// Need to enforce that Select must only come after a terminator
			else if (prevToken.type == QueryToken::QueryTokenType::TERMINATOR && token.token_value == "Select") {
				temp = { QueryToken::QueryTokenType::SELECT, "Select" };
			}
		}

		// Guard clauses to catch semantically wrong input, all usage of token should be temp
		if (prevToken.type == QueryToken::QueryTokenType::IDENTIFIER &&
			(token.type != QueryToken::QueryTokenType::COMMA &&
				token.type != QueryToken::QueryTokenType::TERMINATOR)) {
			throw std::runtime_error("During declaration, only comma and terminator is accepted after identifier.");
		}
		if (prevToken.type == QueryToken::QueryTokenType::COMMA &&
			token.type != QueryToken::QueryTokenType::IDENTIFIER) {
			throw std::runtime_error("During declaration, only identifier is accepted after comma.");
		}
		if (token.type == QueryToken::QueryTokenType::TERMINATOR) {
			temp = token;
		}

		Entity ent;
		if (prevToken.type != QueryToken::QueryTokenType::WHITESPACE &&
			prevToken.type != QueryToken::QueryTokenType::TERMINATOR &&
			token.type == QueryToken::QueryTokenType::IDENTIFIER) {
			// Declaring goes into output
			if (prevToken.type != QueryToken::QueryTokenType::SELECT) {
				output.push_back({ prevToken.type, { token.token_value } });

				// TODO: Add into entity
				if (prevToken.type == QueryToken::QueryTokenType::STMT) {
					ent = { EntityType::STMT, { token.token_value } };
				}
				else if (prevToken.type == QueryToken::QueryTokenType::PROCEDURE) {
					ent = { EntityType::PROCEDURE, { token.token_value } };
				}
				else if (prevToken.type == QueryToken::QueryTokenType::READ) {
					ent = { EntityType::READ, { token.token_value } };
				}
				else if (prevToken.type == QueryToken::QueryTokenType::PRINT) {
					ent = { EntityType::PRINT, { token.token_value } };
				}
				else if (prevToken.type == QueryToken::QueryTokenType::CALL) {
					ent = { EntityType::CALL, { token.token_value } };
				}
				else if (prevToken.type == QueryToken::QueryTokenType::IF) {
					ent = { EntityType::IF, { token.token_value } };
				}
				else if (prevToken.type == QueryToken::QueryTokenType::WHILE) {
					ent = { EntityType::WHILE, { token.token_value } };
				}
				else if (prevToken.type == QueryToken::QueryTokenType::ASSIGN) {
					ent = { EntityType::ASSIGN, { token.token_value } };
				}
				else if (prevToken.type == QueryToken::QueryTokenType::VARIABLE) {
					ent = { EntityType::VARIABLE, { token.token_value } };
				}
				else if (prevToken.type == QueryToken::QueryTokenType::CONSTANT) {
					ent = { EntityType::CONSTANT, token.token_value };
				}
				query.addEntity(ent);
			}
			// Select goes into selected, with prevToken type when it exists in output
			else {
				for (QueryToken each : output) {
					if (token.token_value == each.token_value) {
						selected.push_back({ each.type, token.token_value });
						if (each.type == QueryToken::QueryTokenType::STMT) {
							ent = { EntityType::STMT, { token.token_value } };
						}
						else if (each.type == QueryToken::QueryTokenType::PROCEDURE) {
							ent = { EntityType::PROCEDURE, { token.token_value } };
						}
						else if (each.type == QueryToken::QueryTokenType::READ) {
							ent = { EntityType::READ, { token.token_value } };
						}
						else if (each.type == QueryToken::QueryTokenType::PRINT) {
							ent = { EntityType::PRINT, { token.token_value } };
						}
						else if (each.type == QueryToken::QueryTokenType::CALL) {
							ent = { EntityType::CALL, { token.token_value } };
						}
						else if (each.type == QueryToken::QueryTokenType::IF) {
							ent = { EntityType::IF, { token.token_value } };
						}
						else if (each.type == QueryToken::QueryTokenType::WHILE) {
							ent = { EntityType::WHILE, { token.token_value } };
						}
						else if (each.type == QueryToken::QueryTokenType::ASSIGN) {
							ent = { EntityType::ASSIGN, { token.token_value } };
						}
						else if (each.type == QueryToken::QueryTokenType::VARIABLE) {
							ent = { EntityType::VARIABLE, { token.token_value } };
						}
					}
				}
				query.addSelected(ent);
			}
		}
		prevToken = temp;
	}

	return query;
}
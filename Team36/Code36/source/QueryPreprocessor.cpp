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

	//query_tokenizer.parse_into_query_tokens(str);

	//const std::vector<QueryToken> v = query_tokenizer.get_query_token_chain();

	std::vector<QueryToken> v;
	//QueryToken q1(QueryToken::QueryTokenType::IDENTIFIER, "stmt");
	//v.push_back(q1);
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "s" });
	//v.push_back({ QueryToken::QueryTokenType::COMMA, "" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "ss" });
	//v.push_back({ QueryToken::QueryTokenType::TERMINATOR, ";" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "procedure" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "p" });
	//v.push_back({ QueryToken::QueryTokenType::TERMINATOR, ";" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "Select" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "s" });
	//v.push_back({ QueryToken::QueryTokenType::COMMA, "" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "p" });
	v.push_back({ QueryToken::QueryTokenType::SUCHTHAT, "" });
	v.push_back({ QueryToken::QueryTokenType::PARENT_T, "" });
	v.push_back({ QueryToken::QueryTokenType::PARENTHESIS_OPEN, "" });
	v.push_back({ QueryToken::QueryTokenType::CONSTANT, "7" });
	v.push_back({ QueryToken::QueryTokenType::COMMA, "" });
	v.push_back({ QueryToken::QueryTokenType::WILDCARD, "" });
	v.push_back({ QueryToken::QueryTokenType::PARENTHESIS_CLOSE, "" });

	std::vector<QueryToken> output;
	std::vector<QueryToken> selected;
	std::vector<QueryToken> parameterClause;
	QueryToken prevToken;
	QueryToken temp;
	QueryToken prevTokenSelect;
	QueryToken queryParameter;
	QueryToken patternOrSuchThat;
	bool isSelect = true;
	bool isParameter = false;

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
				Synonym synonym;
				if (prevToken.type == QueryToken::QueryTokenType::STMT) {
					synonym.name = token.token_value;
					ent = { EntityType::STMT, synonym };
				}
				else if (prevToken.type == QueryToken::QueryTokenType::PROCEDURE) {
					synonym.name = token.token_value;
					ent = { EntityType::PROCEDURE, synonym };
				}
				else if (prevToken.type == QueryToken::QueryTokenType::READ) {
					synonym.name = token.token_value;
					ent = { EntityType::READ, synonym };
				}
				else if (prevToken.type == QueryToken::QueryTokenType::PRINT) {
					synonym.name = token.token_value;
					ent = { EntityType::PRINT, synonym };
				}
				else if (prevToken.type == QueryToken::QueryTokenType::CALL) {
					synonym.name = token.token_value;
					ent = { EntityType::CALL, synonym };
				}
				else if (prevToken.type == QueryToken::QueryTokenType::IF) {
					synonym.name = token.token_value;
					ent = { EntityType::IF, synonym };
				}
				else if (prevToken.type == QueryToken::QueryTokenType::WHILE) {
					synonym.name = token.token_value;
					ent = { EntityType::WHILE, synonym };
				}
				else if (prevToken.type == QueryToken::QueryTokenType::ASSIGN) {
					synonym.name = token.token_value;
					ent = { EntityType::ASSIGN, synonym };
				}
				else if (prevToken.type == QueryToken::QueryTokenType::VARIABLE) {
					synonym.name = token.token_value;
					ent = { EntityType::VARIABLE, synonym };
				}
				else if (prevToken.type == QueryToken::QueryTokenType::CONSTANT) {
					synonym.name = token.token_value;
					ent = { EntityType::CONSTANT, synonym };
				}
				query.addEntity(ent);
			}
			// Select goes into selected, with prevToken type when it exists in output
			else {
				for (QueryToken each : output) {
					if (token.token_value == each.token_value) {
						selected.push_back({ each.type, token.token_value });
						Synonym synonym;
						if (each.type == QueryToken::QueryTokenType::STMT) {
							synonym.name = token.token_value;
							ent = { EntityType::STMT, synonym };
						}
						else if (each.type == QueryToken::QueryTokenType::PROCEDURE) {
							synonym.name = token.token_value;
							ent = { EntityType::PROCEDURE, synonym };
						}
						else if (each.type == QueryToken::QueryTokenType::READ) {
							synonym.name = token.token_value;
							ent = { EntityType::READ, synonym };
						}
						else if (each.type == QueryToken::QueryTokenType::PRINT) {
							synonym.name = token.token_value;
							ent = { EntityType::PRINT, synonym };
						}
						else if (each.type == QueryToken::QueryTokenType::CALL) {
							synonym.name = token.token_value;
							ent = { EntityType::CALL, synonym };
						}
						else if (each.type == QueryToken::QueryTokenType::IF) {
							synonym.name = token.token_value;
							ent = { EntityType::IF, synonym };
						}
						else if (each.type == QueryToken::QueryTokenType::WHILE) {
							synonym.name = token.token_value;
							ent = { EntityType::WHILE, synonym };
						}
						else if (each.type == QueryToken::QueryTokenType::ASSIGN) {
							synonym.name = token.token_value;
							ent = { EntityType::ASSIGN, synonym };
						}
						else if (each.type == QueryToken::QueryTokenType::VARIABLE) {
							synonym.name = token.token_value;
							ent = { EntityType::VARIABLE, synonym };
						}
					}
				}
				isSelect = true;
				query.addSelected(ent);
			}
		}

		if (isSelect) {
			if (token.type == QueryToken::QueryTokenType::SUCHTHAT) {
				output.push_back({ QueryToken::QueryTokenType::SUCHTHAT, "such that" });
				patternOrSuchThat = { QueryToken::QueryTokenType::SUCHTHAT, "such that" };
			}
			else if (token.type == QueryToken::QueryTokenType::PATTERN) {
				output.push_back({ QueryToken::QueryTokenType::PATTERN, "pattern" });
				patternOrSuchThat = { QueryToken::QueryTokenType::PATTERN, "pattern" };
			}

			if (isParameter) {
				parameterClause.push_back(token);
			}

			if (!isParameter && patternOrSuchThat.type == QueryToken::QueryTokenType::SUCHTHAT) {
				if (token.type == QueryToken::QueryTokenType::PARENT_T) {
					output.push_back({ QueryToken::QueryTokenType::PARENT_T, "Parent_T" });
				}
				else if (token.type == QueryToken::QueryTokenType::FOLLOWS_T) {
					output.push_back({ QueryToken::QueryTokenType::FOLLOWS_T, "Follows_T" });
				}
				else if (token.type == QueryToken::QueryTokenType::PARENTHESIS_OPEN) {
					if (prevTokenSelect.token_value == "Uses" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
						queryParameter.token_value = "Uses";
					}
					else if (prevTokenSelect.token_value == "Modifies" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
						queryParameter.token_value = "Modifies";
					}
					else if (prevTokenSelect.token_value == "Parent" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
						queryParameter.token_value = "Parent";
					}
					else if (prevTokenSelect.token_value == "Follows" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
						queryParameter.token_value = "Follows";
					}
					isParameter == true;
				}
				else if (token.type == QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
					if (queryParameter.token_value == "Uses") {
					}
					else if (queryParameter.token_value == "Modifies") {
					}
					else if (queryParameter.token_value == "Parent") {
					}
					else if (queryParameter.token_value == "Follows") {
					}
					isParameter == false;
				}
			}
			else if (!isParameter && patternOrSuchThat.type == QueryToken::QueryTokenType::PATTERN) {
			}
			prevTokenSelect = token;
		}

		prevToken = temp;
	}

	return query;
}
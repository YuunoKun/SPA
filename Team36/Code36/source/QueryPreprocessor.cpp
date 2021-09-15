#include "QueryPreprocessor.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "Entity.h"
#include "QueryToken.h"
#include "QueryTokenizer.h"
#include "Query.h"
#include "PatternRelRefValidator.h"
#include "Utility.h"

QueryPreprocessor::QueryPreprocessor() {
}

Query QueryPreprocessor::parse(std::string str) {
	Query query = Query();
	QueryTokenizer query_tokenizer;
	PatternRelRefValidator validator;

	query_tokenizer.parse_into_query_tokens(str);

	const std::vector<QueryToken> v = query_tokenizer.get_query_token_chain();

	//std::vector<QueryToken> v;
	//QueryToken q1(QueryToken::QueryTokenType::IDENTIFIER, "stmt");
	//v.push_back(q1);
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "s" });
	//v.push_back({ QueryToken::QueryTokenType::COMMA, "" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "ss" });
	//v.push_back({ QueryToken::QueryTokenType::TERMINATOR, ";" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "assign" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "a" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "procedure" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "p" });
	//v.push_back({ QueryToken::QueryTokenType::TERMINATOR, ";" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "Select" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "p" });
	//v.push_back({ QueryToken::QueryTokenType::COMMA, "" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "p" });
	//v.push_back({ QueryToken::QueryTokenType::SUCH_THAT, "" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "Modifies" });
	//v.push_back({ QueryToken::QueryTokenType::PARENTHESIS_OPEN, "" });
	//v.push_back({ QueryToken::QueryTokenType::CONSTANT, "7" });
	//v.push_back({ QueryToken::QueryTokenType::COMMA, "" });
	//v.push_back({ QueryToken::QueryTokenType::WILDCARD, "" });
	//v.push_back({ QueryToken::QueryTokenType::PARENTHESIS_CLOSE, "" });
	//v.push_back({ QueryToken::QueryTokenType::PATTERN, "" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "a" });
	//v.push_back({ QueryToken::QueryTokenType::PARENTHESIS_OPEN, "" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "" });

	// Collection of declared query tokens
	std::vector<QueryToken> output;

	// Collection of selected query tokens
	std::vector<QueryToken> selected;

	// To keep track of parameter content in such that and pattern clauses
	std::vector<QueryToken> parameterClause;

	// To keep track of previous token during declaration, includes COMMA and SEMICOLON
	QueryToken prevToken = QueryToken();

	// To keep track of of previous important declaration token, excludes COMMA and SEMICOLON
	QueryToken temp = QueryToken();

	// To keep track of previous token during Selection
	QueryToken prevTokenSelect = QueryToken();

	// To keep track of valid such that clause type, eg: Uses, Modifies, Parent, ParentT
	QueryToken queryParameter = QueryToken();

	// To keep track if selection is in pattern or such that
	QueryToken patternOrSuchThat = QueryToken();

	// To keep track of pattern type entity, eg: ASSIGN
	Entity patternTypeEntity;

	// True when parsing is in selection, false when parsing is in declaration
	bool isSelect = false;

	// True when iterating inside the such that or pattern parameter, false otherwise
	bool isParameter = false;

	for (QueryToken token : v) {
		// First iteration, set identifier to correct type
		// Check what is my previous token
		if (!isSelect) {
			temp = setIdentifierToQueryTokenType(prevToken, temp, token);

			validateDeclarationQuery(prevToken, token);

			if (token.type == QueryToken::QueryTokenType::TERMINATOR) {
				temp = token;
			}

			Entity ent;
			if (prevToken.type != QueryToken::QueryTokenType::WHITESPACE &&
				prevToken.type != QueryToken::QueryTokenType::TERMINATOR &&
				token.type == QueryToken::QueryTokenType::IDENTIFIER) {
				// If declaring add into entity
				if (prevToken.type != QueryToken::QueryTokenType::SELECT) {
					addEntityToQuery(query, ent, output, prevToken, token);
				}
				// If select add into selected
				else {
					addSelectedToQuery(query, ent, output, selected, prevToken, token, isSelect);
				}
			}
			prevToken = temp;
		}

		else if (isSelect) {
			if (token.type == QueryToken::QueryTokenType::SUCH_THAT) {
				patternOrSuchThat = { QueryToken::QueryTokenType::SUCH_THAT, "" };
			}
			else if (token.type == QueryToken::QueryTokenType::IDENTIFIER && token.token_value == "pattern") {
				patternOrSuchThat = { QueryToken::QueryTokenType::PATTERN, "pattern" };
			}

			if (isParameter && token.type != QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
				parameterClause.push_back(token);
			}

			if (patternOrSuchThat.type == QueryToken::QueryTokenType::SUCH_THAT) {
				if (!isParameter && token.type == QueryToken::QueryTokenType::PARENTHESIS_OPEN) {
					isParameter = true;
					setQueryParameter(prevTokenSelect, queryParameter);
				}
				else if (token.type == QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
					isParameter = false;
					validator.parseParameterSuchThat(query, queryParameter.type, parameterClause);
					parameterClause.clear();
				}
			}
			else if (patternOrSuchThat.type == QueryToken::QueryTokenType::PATTERN) {
				if (prevTokenSelect.token_value == "pattern" && token.type == QueryToken::QueryTokenType::IDENTIFIER) {
					bool isValid = false;
					for (QueryToken each : output) {
						if (token.token_value == each.token_value) {
							Synonym synonym;
							synonym.name = token.token_value;
							EntityType entityType = Utility::queryTokenTypeToEntityType(each.type);
							patternTypeEntity = { entityType, synonym };
							isValid = true;
						}
					}
					if (!isValid) {
						throw std::runtime_error("Pattern type has not been declared");
					}
				}
				else if (!isParameter && token.type == QueryToken::QueryTokenType::PARENTHESIS_OPEN) {
					isParameter = true;
				}
				else if (isParameter && token.type == QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
					isParameter = false;
					if (patternTypeEntity.getType() == EntityType::ASSIGN) {
						validator.parseParameterPattern(query, patternTypeEntity, parameterClause);
						parameterClause.clear();
					}
					else {
						throw std::runtime_error("Invalid pattern type");
					}
				}
			}
			prevTokenSelect = token;
		}
	}
	return query;
}

QueryToken QueryPreprocessor::setIdentifierToQueryTokenType(QueryToken& prevToken, QueryToken& temp, QueryToken& token) {
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
	return temp;
}

void QueryPreprocessor::validateDeclarationQuery(QueryToken& prevToken, QueryToken& token) {
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
}

void QueryPreprocessor::addEntityToQuery(Query& query, Entity& ent, std::vector<QueryToken>& output, QueryToken& prevToken, QueryToken& token) {
	// Declaring goes into output
	output.push_back({ prevToken.type, { token.token_value } });
	Synonym synonym;
	synonym.name = token.token_value;
	EntityType entityType = Utility::queryTokenTypeToEntityType(prevToken.type);
	ent = { entityType, synonym };
	query.addEntity(ent);
}

void QueryPreprocessor::addSelectedToQuery(Query& query, Entity& ent, std::vector<QueryToken>& output, std::vector<QueryToken> selected, QueryToken& prevToken, QueryToken& token, bool& isSelect) {
	bool isValid = false;
	for (QueryToken each : output) {
		if (token.token_value == each.token_value) {
			selected.push_back({ each.type, token.token_value });
			Synonym synonym;
			synonym.name = token.token_value;
			EntityType entityType = Utility::queryTokenTypeToEntityType(each.type);
			ent = { entityType, synonym };
			isValid = true;
		}
	}
	if (!isValid) {
		throw std::runtime_error("Select variable content has not been declared");
	}
	isSelect = true;
	query.addSelected(ent);
}

void QueryPreprocessor::setQueryParameter(QueryToken& prevTokenSelect, QueryToken& queryParameter) {
	if (prevTokenSelect.token_value == "Uses" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
		queryParameter = { QueryToken::QueryTokenType::USES_S, "Uses" };
	}
	else if (prevTokenSelect.token_value == "Modifies" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
		queryParameter = { QueryToken::QueryTokenType::MODIFIES_S, "Modifies" };
	}
	else if (prevTokenSelect.token_value == "Parent" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
		queryParameter = { QueryToken::QueryTokenType::PARENT, "Parent" };
	}
	else if (prevTokenSelect.token_value == "Follows" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
		queryParameter = { QueryToken::QueryTokenType::FOLLOWS, "Follows" };
	}
	else if (prevTokenSelect.token_value == "" && prevTokenSelect.type == QueryToken::QueryTokenType::PARENT_T) {
		queryParameter = { QueryToken::QueryTokenType::PARENT_T, "" };
	}
	else if (prevTokenSelect.token_value == "" && prevTokenSelect.type == QueryToken::QueryTokenType::FOLLOWS_T) {
		queryParameter = { QueryToken::QueryTokenType::FOLLOWS_T, "" };
	}
}
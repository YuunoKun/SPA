#include "QueryPreprocessor.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "Entity.h"
#include "QueryToken.h"
#include "QueryTokenizer.h"
#include "Query.h"
#include "Utility.h"

QueryPreprocessor::QueryPreprocessor() {
}

Entity setStmtRef(Query& query, QueryToken token) {

	// wild card check
	if (token.type == QueryToken::WILDCARD) {
		return Entity(EntityType::WILD);
	}

	// is INTEGER, constant type
	if (token.type == QueryToken::CONSTANT) {
		return Entity(EntityType::CONSTANT, token.token_value);
	}

	// synonym check
	std::unordered_map<std::string, Entity> ent_chain = query.getEntities();
	if (ent_chain.find(token.token_value) != ent_chain.end()) {
		return ent_chain.at(token.token_value);
	}

	std::runtime_error("Unknown stmtRef");
}

Entity setEntRef(Query& query, std::vector<QueryToken> token_chain) {
	if (token_chain.size() == 1) {
		// wild card check
		if (token_chain[0].type == QueryToken::WILDCARD) {
			return Entity(EntityType::WILD);
		}

		// synonym check
		std::unordered_map<std::string, Entity> ent_chain = query.getEntities();
		if (ent_chain.find(token_chain[0].token_value) != ent_chain.end()) {
			return ent_chain.at(token_chain[0].token_value);
		}
	}


	// is " "IDENT" "
	if (token_chain[0].type == QueryToken::QUOTATION_OPEN &&
		token_chain[1].type == QueryToken::IDENTIFIER &&
		token_chain[2].type == QueryToken::QUOTATION_CLOSE) {
		return Entity(EntityType::VARIABLE, token_chain[1].token_value);
	}



	std::runtime_error("Unknown stmtRef");
}

expr setExpr(std::vector<QueryToken> token_chain) {
	//todo
	return "pp";
}

void parseParameterSuchThat(Query& query, QueryToken::QueryTokenType token_type, std::vector<QueryToken> token_chain) {


	switch (token_type) {
	case QueryToken::MODIFIES_S:
		if (token_chain[1].type == QueryToken::COMMA) {
			QueryToken stmt = token_chain[0];
			token_chain.erase(token_chain.begin(), token_chain.begin() + 2);
			query.addRelation(RelRef(RelType::MODIFIES_S,
				setStmtRef(query, token_chain[0]),
				setEntRef(query, token_chain)));
		}
		std::runtime_error("Unexpected parameters for Modifies");

		break;
	case QueryToken::USES_S:
		if (token_chain[1].type == QueryToken::COMMA) {
			QueryToken stmt = token_chain[0];
			token_chain.erase(token_chain.begin(), token_chain.begin() + 2);
			query.addRelation(RelRef(RelType::USES_S,
				setStmtRef(query, token_chain[0]),
				setEntRef(query, token_chain)));
		}
		std::runtime_error("Unexpected parameters for Uses");

		break;

	case QueryToken::PARENT:
		if (token_chain[1].type == QueryToken::COMMA) {
			query.addRelation(RelRef(RelType::PARENT,
				setStmtRef(query, token_chain[0]),
				setStmtRef(query, token_chain[2])));
		}
		std::runtime_error("Unexpected parameters for Parent");

		break;
	case QueryToken::PARENT_T:
		if (token_chain[1].type == QueryToken::COMMA) {
			query.addRelation(RelRef(RelType::PARENT_T,
				setStmtRef(query, token_chain[0]),
				setStmtRef(query, token_chain[2])));
		}
		std::runtime_error("Unexpected parameters for Parent*");

		break;
	case QueryToken::FOLLOWS:
		if (token_chain[1].type == QueryToken::COMMA) {
			query.addRelation(RelRef(RelType::FOLLOWS,
				setStmtRef(query, token_chain[0]),
				setStmtRef(query, token_chain[2])));
		}
		std::runtime_error("Unexpected parameters for Follows");

		break;
	case QueryToken::FOLLOWS_T:
		if (token_chain[1].type == QueryToken::COMMA) {
			query.addRelation(RelRef(RelType::FOLLOWS_T,
				setStmtRef(query, token_chain[0]),
				setStmtRef(query, token_chain[2])));
		}
		std::runtime_error("Unexpected parameters for Follows*");

		break;

	default:
		std::runtime_error("Unknown RelRef query token type : \'" + token_type + '\'');
	}

}

void parseParameterPattern(Query& query, Entity& synonym, std::vector<QueryToken> token_chain) {
	bool wild = false;
	switch (synonym.getType()) {
	case ASSIGN:
		// find comma
		std::vector<QueryToken> temp_token_chain;
		for (QueryToken& t : token_chain) {
			if (t.type != QueryToken::COMMA) {
				temp_token_chain.push_back(token_chain[0]);
				token_chain.erase(token_chain.begin());
			}
			else {
				break;
			}
		}

		if (token_chain[0].type == QueryToken::COMMA) {
			token_chain.erase(token_chain.begin());
		}
		else {
			std::runtime_error("Unexpected parameters for Pattern");
		}

		if (token_chain[0].type == QueryToken::WILDCARD) {
			token_chain.erase(token_chain.begin());
			wild = true;
			if (token_chain[token_chain.size() - 1].type != QueryToken::WILDCARD) {
				std::runtime_error("Unexpected parameters for Pattern");
			}
		}
		else {
			std::runtime_error("Unexpected parameters for Pattern");
		}


		std::vector<QueryToken> temp_token_chain_2;

		if (token_chain[0].type == QueryToken::QUOTATION_OPEN) {
			token_chain.erase(token_chain.begin());

			for (QueryToken& t : token_chain) {
				if (t.type != QueryToken::QUOTATION_CLOSE) {
					temp_token_chain_2.push_back(token_chain[0]);
					token_chain.erase(token_chain.begin());
				}
				else {
					break;
				}
			}
		}

		query.addPattern(Pattern(synonym, setEntRef(query, temp_token_chain), setExpr(temp_token_chain_2), wild));

	}
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
	v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "procedure" });
	v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "p" });
	v.push_back({ QueryToken::QueryTokenType::TERMINATOR, ";" });
	v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "Select" });
	v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "p" });
	//v.push_back({ QueryToken::QueryTokenType::COMMA, "" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "p" });
	//v.push_back({ QueryToken::QueryTokenType::SUCHTHAT, "" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "Modifies" });
	//v.push_back({ QueryToken::QueryTokenType::PARENTHESIS_OPEN, "" });
	//v.push_back({ QueryToken::QueryTokenType::CONSTANT, "7" });
	//v.push_back({ QueryToken::QueryTokenType::COMMA, "" });
	//v.push_back({ QueryToken::QueryTokenType::WILDCARD, "" });
	//v.push_back({ QueryToken::QueryTokenType::PARENTHESIS_CLOSE, "" });

	std::vector<QueryToken> output;
	std::vector<QueryToken> selected;
	std::vector<QueryToken> parameterClause;
	QueryToken prevToken;
	QueryToken temp;
	QueryToken prevTokenSelect;
	QueryToken queryParameter;
	QueryToken patternOrSuchThat;
	bool isSelect = false;
	bool isParameter = false;

	for (QueryToken token : v) {
		// First iteration, set identifier to correct type
		// Check what is my previous token
		if (!isSelect) {
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
					Synonym synonym;
					synonym.name = token.token_value;
					EntityType entityType = Utility::queryTokenTypeToEntityType(prevToken.type);
					ent = { entityType, synonym };
					query.addEntity(ent);
				}
				// Select goes into selected, with prevToken type when it exists in output
				else {
					for (QueryToken each : output) {
						if (token.token_value == each.token_value) {
							selected.push_back({ each.type, token.token_value });
							Synonym synonym;
							synonym.name = token.token_value;
							EntityType entityType = Utility::queryTokenTypeToEntityType(each.type);
							ent = { entityType, synonym };
						}
					}
					isSelect = true;
					query.addSelected(ent);
				}
			}
			prevToken = temp;
		}

		else if (isSelect) {
			if (token.type == QueryToken::QueryTokenType::SUCH_THAT) {
				output.push_back({ QueryToken::QueryTokenType::SUCH_THAT, "such that" });
				patternOrSuchThat = { QueryToken::QueryTokenType::SUCH_THAT, "such that" };
			}
			else if (token.type == QueryToken::QueryTokenType::PATTERN) {
				output.push_back({ QueryToken::QueryTokenType::PATTERN, "pattern" });
				patternOrSuchThat = { QueryToken::QueryTokenType::PATTERN, "pattern" };
			}

			if (isParameter && !token.type == QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
				parameterClause.push_back(token);
			}

			if (!isParameter && patternOrSuchThat.type == QueryToken::QueryTokenType::SUCH_THAT) {
				if (token.type == QueryToken::QueryTokenType::PARENT_T) {
					output.push_back({ QueryToken::QueryTokenType::PARENT_T, "" });
				}
				else if (token.type == QueryToken::QueryTokenType::FOLLOWS_T) {
					output.push_back({ QueryToken::QueryTokenType::FOLLOWS_T, "" });
				}
				else if (token.type == QueryToken::QueryTokenType::PARENTHESIS_OPEN) {
					isParameter = true;
					if (prevTokenSelect.token_value == "Uses" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
						queryParameter = { QueryToken::QueryTokenType::IDENTIFIER, "Uses" };
					}
					else if (prevTokenSelect.token_value == "Modifies" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
						queryParameter = { QueryToken::QueryTokenType::IDENTIFIER, "Modifies" };
					}
					else if (prevTokenSelect.token_value == "Parent" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
						queryParameter = { QueryToken::QueryTokenType::IDENTIFIER, "Parent" };
					}
					else if (prevTokenSelect.token_value == "Follows" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
						queryParameter = { QueryToken::QueryTokenType::IDENTIFIER, "Follows" };
					}
					else if (prevTokenSelect.token_value == "" && prevTokenSelect.type == QueryToken::QueryTokenType::PARENT_T) {
						queryParameter = { QueryToken::QueryTokenType::PARENT_T, "" };
					}
					else if (prevTokenSelect.token_value == "" && prevTokenSelect.type == QueryToken::QueryTokenType::FOLLOWS_T) {
						queryParameter = { QueryToken::QueryTokenType::FOLLOWS_T, "" };
					}
				}
				else if (token.type == QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
					isParameter = false;
					if (queryParameter.token_value == "Uses" && queryParameter.type == QueryToken::QueryTokenType::IDENTIFIER) {
						//query.addRelation()
					}
					else if (queryParameter.token_value == "Modifies" && queryParameter.type == QueryToken::QueryTokenType::IDENTIFIER) {
						//query.addRelation()
					}
					else if (queryParameter.token_value == "Parent" && queryParameter.type == QueryToken::QueryTokenType::IDENTIFIER) {
						//query.addRelation()
					}
					else if (queryParameter.token_value == "Follows" && queryParameter.type == QueryToken::QueryTokenType::IDENTIFIER) {
						//query.addRelation()
					}
					else if (queryParameter.token_value == "" && queryParameter.type == QueryToken::QueryTokenType::PARENT_T) {
						//query.addRelation()
					}
					else if (queryParameter.token_value == "" && queryParameter.type == QueryToken::QueryTokenType::FOLLOWS_T) {
						//query.addRelation()
					}
				}
			}
			else if (!isParameter && patternOrSuchThat.type == QueryToken::QueryTokenType::PATTERN) {
				Entity patternTypeEntity;
				if (prevTokenSelect.type == QueryToken::QueryTokenType::PATTERN && token.type == QueryToken::QueryTokenType::IDENTIFIER) {
					for (QueryToken each : output) {
						if (token.token_value == each.token_value) {
							selected.push_back({ each.type, token.token_value });
							Synonym synonym;
							synonym.name = token.token_value;
							EntityType entityType = Utility::queryTokenTypeToEntityType(each.type);
							patternTypeEntity = { entityType, synonym };
						}
					}
				}
				else if (token.type == QueryToken::QueryTokenType::PARENTHESIS_OPEN) {
					isParameter = true;
				}
				else if (token.type == QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
					isParameter = false;
					if (patternTypeEntity.getType() == EntityType::ASSIGN) {
						// send to jiyu and receive back pattern
						// query.addPattern()
					}
				}
			}
			prevTokenSelect = token;
		}
	}
	return query;
}




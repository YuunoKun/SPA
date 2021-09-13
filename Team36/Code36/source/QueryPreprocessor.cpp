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

Entity QueryPreprocessor::setStmtRef(Query& query, QueryToken token) {
	//synonym | ‘_’ | INTEGER

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

Entity QueryPreprocessor::setEntRef(Query& query, std::vector<QueryToken> token_chain) {
	// entRef : synonym | ‘_’ | ‘"’ IDENT ‘"’

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

// takes in a token_chain with only IDENT* (no QUOTATION_OPEN/CLOSE or WILDCARD)
expr QueryPreprocessor::setExpr(std::vector<QueryToken> token_chain) {
	//expression-spec : ‘"‘ expr’"’ | ‘_’ ‘"’ expr ‘"’ ‘_’ | ‘_’

	//TODO
	// send to expression parser
	if (token_chain.empty()) {
		return "";
	}
	else if (token_chain.size() == 1 && token_chain[0].type == QueryToken::IDENTIFIER) {
		return token_chain[0].token_value;
	}
}

void QueryPreprocessor::parseParameterSuchThat(Query& query, QueryToken::QueryTokenType token_type, std::vector<QueryToken> token_chain) {
	switch (token_type) {
	case QueryToken::MODIFIES_S:
		// stmtRef , entRef
		if (token_chain[1].type == QueryToken::COMMA) {
			QueryToken stmt = token_chain[0];
			token_chain.erase(token_chain.begin(), token_chain.begin() + 2);
			query.addRelation(RelRef(RelType::MODIFIES_S,
				setStmtRef(query, stmt),
				setEntRef(query, token_chain)));
		}
		std::runtime_error("Unexpected parameters for Modifies");

		break;
	case QueryToken::USES_S:
		// stmtRef , entRef
		if (token_chain[1].type == QueryToken::COMMA) {
			QueryToken stmt = token_chain[0];
			token_chain.erase(token_chain.begin(), token_chain.begin() + 2);
			query.addRelation(RelRef(RelType::USES_S,
				setStmtRef(query, stmt),
				setEntRef(query, token_chain)));
		}
		std::runtime_error("Unexpected parameters for Uses");

		break;

	case QueryToken::PARENT:
		// stmtRef , stmtRef
		if (token_chain[1].type == QueryToken::COMMA) {
			query.addRelation(RelRef(RelType::PARENT,
				setStmtRef(query, token_chain[0]),
				setStmtRef(query, token_chain[2])));
		}
		std::runtime_error("Unexpected parameters for Parent");

		break;
	case QueryToken::PARENT_T:
		// stmtRef , stmtRef
		if (token_chain[1].type == QueryToken::COMMA) {
			query.addRelation(RelRef(RelType::PARENT_T,
				setStmtRef(query, token_chain[0]),
				setStmtRef(query, token_chain[2])));
		}
		std::runtime_error("Unexpected parameters for Parent*");

		break;
	case QueryToken::FOLLOWS:
		// stmtRef , stmtRef
		if (token_chain[1].type == QueryToken::COMMA) {
			query.addRelation(RelRef(RelType::FOLLOWS,
				setStmtRef(query, token_chain[0]),
				setStmtRef(query, token_chain[2])));
		}
		std::runtime_error("Unexpected parameters for Follows");

		break;
	case QueryToken::FOLLOWS_T:
		// stmtRef , stmtRef
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

void QueryPreprocessor::parseParameterPattern(Query& query, Entity& synonym_ent, std::vector<QueryToken> token_chain) {
	bool wild = false;
	switch (synonym_ent.getType()) {
	case ASSIGN:
		// find comma
		// every QueryToken before comma token is sent to temp_token_chain
		// Expect entRef token_chain
		std::vector<QueryToken> temp_token_chain;
		for (int i = 0; i < token_chain.size(); i++) {
			if (token_chain[0].type != QueryToken::COMMA) {
				temp_token_chain.push_back(token_chain[0]);
				token_chain.erase(token_chain.begin());
			}
			else {
				break;
			}
		}

		// check if the curr token_chain starts with comma
		if (token_chain[0].type == QueryToken::COMMA) {
			token_chain.erase(token_chain.begin());
		}
		else {
			// if no comma found means invalid parameters
			std::runtime_error("Unexpected parameters for Pattern");
		}

		//check second parameter if is WILDCARD
		if (token_chain[0].type == QueryToken::WILDCARD) {
			wild = true;
			// if first element is WILDCARD token, last element must be WILDCARD for last 2 cases below
			// expression-spec : ‘"‘ expr’"’ | ‘_’ ‘"’ expr ‘"’ ‘_’ | ‘_’
			if (token_chain[token_chain.size() - 1].type != QueryToken::WILDCARD) {
				std::runtime_error("Unexpected parameters for Pattern");
			}
			// remove first WILDCARD token
			token_chain.erase(token_chain.begin());
		}
		else {
			std::runtime_error("Unexpected parameters for Pattern");
		}

		std::vector<QueryToken> temp_token_chain_2;

		if (token_chain.size() != 0) {
			if (token_chain[0].type == QueryToken::QUOTATION_OPEN) {
				token_chain.erase(token_chain.begin());

				for (int i = 0; i < token_chain.size(); i++) {
					if (token_chain[0].type != QueryToken::QUOTATION_CLOSE) {
						temp_token_chain_2.push_back(token_chain[0]);
						token_chain.erase(token_chain.begin());
					}
					else {
						break;
					}
				}
			}
			else if (wild && token_chain[0].type == QueryToken::WILDCARD) {
				// 2 WILDCARDS in a row
				std::runtime_error("Unexpected parameters for Pattern");
			}
		}

		query.addPattern(Pattern(synonym_ent, setEntRef(query, temp_token_chain), setExpr(temp_token_chain_2), wild));
	}
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
	QueryToken prevToken;

	// To keep track of of previous important declaration token, excludes COMMA and SEMICOLON
	QueryToken temp;

	// To keep track of previous token during Selection
	QueryToken prevTokenSelect;

	// To keep track of valid such that clause type, eg: Uses, Modifies, Parent, ParentT
	QueryToken queryParameter;

	// To keep track if selection is in pattern or such that
	QueryToken patternOrSuchThat;

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
				output.push_back({ QueryToken::QueryTokenType::SUCH_THAT, "such that" });
				patternOrSuchThat = { QueryToken::QueryTokenType::SUCH_THAT, "such that" };
			}
			else if (token.type == QueryToken::QueryTokenType::IDENTIFIER && token.token_value == "pattern") {
				output.push_back({ QueryToken::QueryTokenType::PATTERN, "pattern" });
				patternOrSuchThat = { QueryToken::QueryTokenType::PATTERN, "pattern" };
			}

			if (isParameter && token.type != QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
				parameterClause.push_back(token);
			}

			if (patternOrSuchThat.type == QueryToken::QueryTokenType::SUCH_THAT) {
				if (token.type == QueryToken::QueryTokenType::PARENT_T) {
					output.push_back({ QueryToken::QueryTokenType::PARENT_T, "" });
				}
				else if (token.type == QueryToken::QueryTokenType::FOLLOWS_T) {
					output.push_back({ QueryToken::QueryTokenType::FOLLOWS_T, "" });
				}
				else if (!isParameter && token.type == QueryToken::QueryTokenType::PARENTHESIS_OPEN) {
					isParameter = true;
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
				else if (token.type == QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
					isParameter = false;
					parseParameterSuchThat(query, queryParameter.type, parameterClause);
				}
			}
			else if (patternOrSuchThat.type == QueryToken::QueryTokenType::PATTERN) {
				if (prevTokenSelect.token_value == "pattern" && token.type == QueryToken::QueryTokenType::IDENTIFIER) {
					for (QueryToken each : output) {
						if (token.token_value == each.token_value) {
							Synonym synonym;
							synonym.name = token.token_value;
							EntityType entityType = Utility::queryTokenTypeToEntityType(each.type);
							patternTypeEntity = { entityType, synonym };
						}
					}
				}
				else if (!isParameter && token.type == QueryToken::QueryTokenType::PARENTHESIS_OPEN) {
					isParameter = true;
				}
				else if (isParameter && token.type == QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
					isParameter = false;
					if (patternTypeEntity.getType() == EntityType::ASSIGN) {
						parseParameterPattern(query, patternTypeEntity, parameterClause);
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
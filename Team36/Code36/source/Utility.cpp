#include <algorithm>
#include <sstream>

#include "Utility.h"


void Utility::constantToStringList(std::vector<constant>& from, std::list<std::string>& out) {
	for (auto& it : from) {
		out.push_back(std::to_string(it));
	}
}

void Utility::procedureToStringList(std::vector<proc_name>& from, std::list<std::string>& out) {
	for (auto& it : from) {
		out.push_back(it.c_str());
	}
}

void Utility::stmtInfoToStringList(std::vector<StmtInfo>& from, std::list<std::string>& out) {
	for (auto& it : from) {
		out.push_back(std::to_string(it.stmt_index));
	}
}
void Utility::stmtIndexToStringList(std::vector<stmt_index>& from, std::list<std::string>& out) {
	for (auto& it : from) {
		out.push_back(std::to_string(it));
	}
}

void Utility::variableToStringList(std::vector<var_name>& from, std::list<std::string>& out) {
	for (auto& it : from) {
		out.push_back(it.c_str());
	}
}

void Utility::unorderedSetToStringList(std::unordered_set<std::string>& from, std::list<std::string>& out) {
	out.insert(out.end(), from.begin(), from.end());
}


std::vector<std::pair<std::string, std::string>> Utility::duplicateColumn(std::vector<std::string>& from) {
	std::vector<std::pair<std::string, std::string>> out;
	for (auto& it : from) {
		out.push_back({ it, it });
	}
	return out;
}

std::vector<std::pair<value, value>> Utility::duplicateColumn(std::vector<value>& from) {
	std::vector<std::pair<value, value>> out;
	for (auto& it : from) {
		out.push_back({ it, it });
	}
	return out;
}

std::vector<std::pair<StmtInfo, StmtInfo>> Utility::duplicateColumn(std::vector<StmtInfo>& from) {
	std::vector<std::pair<StmtInfo, StmtInfo>> out;
	for (auto& it : from) {
		out.push_back({ it, it });
	}
	return out;
}

void Utility::constantToTable(std::vector<constant>& in, std::list<std::vector<value>>& out) {
	for (auto& it : in) {
		out.push_back({ it });
	}
}

void Utility::stringToTable(std::vector<std::string>& in, std::list<std::vector<value>>& out, std::unordered_map<value, std::string>& hash_storage) {
	for (auto& it : in) {
		value hash = hashString(it, hash_storage);
		out.push_back({ hash });
	}
}

void Utility::stringToTable(std::list<std::string>& in, std::list<std::vector<value>>& out, std::unordered_map<value, std::string>& hash_storage) {
	for (auto& it : in) {
		value hash = hashString(it, hash_storage);
		out.push_back({ hash });
	}
}

void Utility::stmtInfoToTable(std::vector<StmtInfo>& in, std::list<std::vector<value>>& out) {
	for (auto& it : in) {
		out.push_back({ it.stmt_index });
	}
}

void Utility::stmtIndexToTable(std::vector<stmt_index>& in, std::list<std::vector<value>>& out) {
	for (auto& it : in) {
		out.push_back({ it });
	}
}

void Utility::pairToTable(std::vector<std::pair<std::string, std::string>>& from, std::list<std::vector<value>>& out,
	std::unordered_map<value, std::string>& hash_storage) {
	for (auto& it : from) {
		value hash1 = hashString(it.first, hash_storage);
		value hash2 = hashString(it.second, hash_storage);
		out.push_back({ hash1, hash2 });
	}
}

void Utility::pairToTable(std::vector<std::pair<StmtInfo, StmtInfo>>& from,
	std::list<std::vector<value>>& out) {
	for (auto& it : from) {
		out.push_back({ it.first.stmt_index, it.second.stmt_index });
	}
}

void Utility::pairToTable(std::vector<std::pair<value, value>>& from,
	std::list<std::vector<value>>& out) {
	for (auto& it : from) {
		out.push_back({ it.first, it.second });
	}
}

void Utility::pairToTable(std::vector<std::pair<StmtInfo, std::string>>& from, std::list<std::vector<value>>& out,
	std::unordered_map<value, std::string>& hash_storage) {
	for (auto& it : from) {
		value hash = hashString(it.second, hash_storage);
		out.push_back({ it.first.stmt_index, hash });
	}
}

void Utility::pairToTable(std::vector<std::pair<value, std::string>>& from, std::list<std::vector<value>>& out,
	std::unordered_map<value, std::string>& hash_storage) {
	for (auto& it : from) {
		value hash = hashString(it.second, hash_storage);
		out.push_back({ it.first, hash });
	}
}

int Utility::getIndex(std::vector<Entity> v, Entity s) {
	auto it = std::find(v.begin(), v.end(), s);
	if (it != v.end()) {
		int index = it - v.begin();
		return index;
	} else {
		return -1;
	}
}

StmtType Utility::convertType(EntityType e) {
	switch (e) {
	case EntityType::STMT:
	case EntityType::PROG_LINE:
	case EntityType::WHILE: return StmtType::STMT_WHILE;
	case EntityType::IF: return StmtType::STMT_IF;
	case EntityType::READ: return StmtType::STMT_READ;
	case EntityType::PRINT: return StmtType::STMT_PRINT;
	case EntityType::CALL: return StmtType::STMT_CALL;
	case EntityType::ASSIGN: return StmtType::STMT_ASSIGN;
	default: throw std::invalid_argument("convert(): does not support this type " + e);
	}
}

std::vector<StmtInfo> Utility::filterResult(EntityType e, std::vector<StmtInfo>& v) {
	if (isStmt(e)) {
		return v;
	}

	StmtType type = convertType(e);;

	std::vector<StmtInfo> result;
	for (auto& it : v) {
		if (it.stmt_type == type) {
			result.push_back(it);
		}
	}

	return  result;
}

void Utility::filterResults(EntityType e, std::vector<StmtInfo>& table, std::list<std::vector<value>>& out) {
	if (isStmt(e)) {
		stmtInfoToTable(table, out);
		return;
	}

	StmtType type = convertType(e);;
	for (auto& it : table) {
		if (it.stmt_type == type) {
			out.push_back({ it.stmt_index });
		}
	}
}

void Utility::filterResults(EntityType e, std::vector<std::pair<StmtInfo, std::string>>& table, std::list<std::vector<value>>& out,
	std::unordered_map<value, std::string>& hash_storage) {
	if (isStmt(e)) {
		pairToTable(table, out, hash_storage);
		return;
	}

	StmtType type = convertType(e);;
	for (auto& it : table) {
		if (it.first.stmt_type == type) {
			value hash = hashString(it.second, hash_storage);
			out.push_back({ it.first.stmt_index, hash });
		}
	}
}

void Utility::filterResults(std::pair<EntityType, EntityType> type, std::vector<std::pair<StmtInfo, StmtInfo>>& table,
	std::list<std::vector<value>>& out) {
	if (isStmt(type.first) && isStmt(type.second)) {
		pairToTable(table, out);
		return;
	}

	StmtType type1 = convertType(type.first);
	StmtType type2 = convertType(type.second);
	for (auto& row : table) {
		if ((isStmt(type.first) || type1 == row.first.stmt_type) &&
			(isStmt(type.second) || type2 == row.second.stmt_type)) {
			out.push_back({ row.first.stmt_index, row.second.stmt_index });
		}
	}
}

void Utility::filterResults(
	std::list<std::vector<value>>& from, std::unordered_set<value>& filter, int index,
	std::list<std::vector<value>>& out) {
	for (auto& it : from) {
		if (filter.find(it[index]) != filter.end()) {
			out.push_back(it);
		}
	}
}

void Utility::filterResults(std::list<std::vector<value>>& from,
	std::unordered_map<value, std::unordered_set<value>>& filter, int index1, int index2,
	std::list<std::vector<value>>& out) {
	for (auto& v : from) {
		auto it1 = filter.find(v[index1]);
		if (it1 != filter.end()) {
			if (it1->second.find(v[index2]) != it1->second.end()) {
				out.push_back(v);
			}
		}
	}
}

void Utility::mergeColumnEqual(std::list<std::vector<value>>& table) {
	auto i = table.begin();
	while (i != table.end()) {
		bool not_equal = false;
		for (unsigned int j = 1; j < i->size(); j++) {
			if (i->at(j - 1) != i->at(j)) {
				not_equal = true;
				break;
			}
		}
		if (not_equal) {
			table.erase(i++);
			continue;
		}
		while (i->size() > 1) {
			i->pop_back();
		}
		i++;
	}
}


void Utility::joinTable(std::list<std::vector<value>>& main, int main_header_index, std::unordered_multimap<value, std::vector<value>>& to_join, int to_join_header_index,
	std::list<std::vector<value>>& out) {
	for (auto& it : main) {
		if (to_join.count(it[main_header_index]) == 0) {
			continue;
		}
		auto& ret = to_join.equal_range(it[main_header_index]);

		for (auto& itr1 = ret.first; itr1 != ret.second; ++itr1) {
			std::vector<value> joined_row;
			joinRow(it, itr1->second, to_join_header_index, joined_row);
			out.emplace_back(joined_row);
		}
	}
}

void Utility::joinTable(std::list<std::vector<value>>& main, std::list<std::vector<value>>& to_join, std::list<std::vector<value>>& out) {
	for (auto& main_row : main) {
		for (auto& to_join_row : to_join) {
			std::vector<value> joined_row;
			joinRow(main_row, to_join_row, joined_row);
			out.emplace_back(joined_row);
		}
	}
}


void Utility::joinRow(std::vector<value>& main, std::vector<value>& to_join, std::vector<value>& out) {
	out.insert(out.end(), main.begin(), main.end());
	for (unsigned int i = 0; i < to_join.size(); i++) {
		out.push_back(to_join[i]);
	}
}

void Utility::joinRow(std::vector<value>& main, std::vector<value>& to_join, int common_index, std::vector<value>& out) {
	out.insert(out.end(), main.begin(), main.end());
	for (unsigned int i = 0; i < to_join.size(); i++) {
		if (i == common_index) {
			continue;
		}
		out.push_back(to_join[i]);
	}
}


std::vector<Entity> Utility::getEntitiesExclude(std::vector<Entity>& main, std::vector<Entity>& to_remove) {

	std::unordered_set<std::string> to_remove_set;
	for (auto& e : to_remove) {
		to_remove_set.insert(e.getSynonym());
	}

	std::vector<Entity> result;

	for (auto& e : main) {
		if (to_remove_set.count(e.getSynonym()) == 0) {
			result.emplace_back(e);
		}
	}

	return result;
}

std::vector<Entity> Utility::getEntitiesWithoutDuplicate(std::vector<Entity>& main) {
	std::unordered_set<std::string> unique;
	std::vector<Entity> result;

	for (auto& e : main) {
		if (unique.count(e.getSynonym()) == 0) {
			result.emplace_back(e);
			unique.insert(e.getSynonym());
		}
	}
	return result;
}

void Utility::getColumnsWithoutDuplicate(std::list<std::vector<value>>& main, std::vector<int>& indexes, std::list<std::vector<value>>& out) {
	std::unordered_set<std::string> unique;
	for (auto& row : main) {
		std::vector<value> newRow;
		std::string rowString;
		for (int i = 0; i < indexes.size(); i++) {
			newRow.emplace_back(row[indexes[i]]);
			rowString += std::to_string(row[indexes[i]]) + " ";
		}
		if (unique.count(rowString) == 0) {
			out.emplace_back(newRow);
			unique.insert(rowString);
		}
	}
}

bool Utility::isSecondaryAttribute(Entity e) {
	return (e.getType() == EntityType::CALL && e.getAttribute() == AttrRef::PROC_NAME) ||
		(e.getType() == EntityType::READ && e.getAttribute() == AttrRef::VAR_NAME) ||
		(e.getType() == EntityType::PRINT && e.getAttribute() == AttrRef::VAR_NAME);
}

EntityType Utility::queryTokenTypeToEntityType(QueryToken::QueryTokenType& query_token_type) {
	if (query_token_type == QueryToken::QueryTokenType::STMT) {
		return EntityType::STMT;
	} else if (query_token_type == QueryToken::QueryTokenType::PROCEDURE) {
		return EntityType::PROCEDURE;
	} else if (query_token_type == QueryToken::QueryTokenType::READ) {
		return EntityType::READ;
	} else if (query_token_type == QueryToken::QueryTokenType::PRINT) {
		return EntityType::PRINT;
	} else if (query_token_type == QueryToken::QueryTokenType::CALL) {
		return EntityType::CALL;
	} else if (query_token_type == QueryToken::QueryTokenType::IF) {
		return EntityType::IF;
	} else if (query_token_type == QueryToken::QueryTokenType::WHILE) {
		return EntityType::WHILE;
	} else if (query_token_type == QueryToken::QueryTokenType::ASSIGN) {
		return EntityType::ASSIGN;
	} else if (query_token_type == QueryToken::QueryTokenType::VARIABLE) {
		return EntityType::VARIABLE;
	} else if (query_token_type == QueryToken::QueryTokenType::CONSTANT) {
		return EntityType::CONSTANT;
	} else if (query_token_type == QueryToken::QueryTokenType::PROG_LINE) {
		return EntityType::PROG_LINE;
	}
}

AttrRef Utility::queryTokenTypeToAttrRef(QueryToken::QueryTokenType& query_token_type) {
	if (query_token_type == QueryToken::QueryTokenType::PROC_NAME) {
		return AttrRef::PROC_NAME;
	} else if (query_token_type == QueryToken::QueryTokenType::VAR_NAME) {
		return AttrRef::VAR_NAME;
	} else if (query_token_type == QueryToken::QueryTokenType::VALUE) {
		return AttrRef::VALUE;
	} else if (query_token_type == QueryToken::QueryTokenType::STMT_INDEX) {
		return AttrRef::STMT_INDEX;
	}
}

std::string Utility::queryTokenTypeToExprString(std::vector<QueryToken> token_chain) {
	std::string result = "";
	for (QueryToken q : token_chain) {
		switch (q.type) {
		case QueryToken::IDENTIFIER:
		case QueryToken::CONSTANT:
			//white space added for every token to prevent 2 separate constants/identifiers in becoming one
			result += ' ';
			result += q.token_value;
			break;
		case QueryToken::PARENTHESIS_OPEN:
			result += '(';
			break;
		case QueryToken::PARENTHESIS_CLOSE:
			result += ')';
			break;
		case QueryToken::PLUS:
			result += '+';
			break;
		case QueryToken::MINUS:
			result += '-';
			break;
		case QueryToken::MUL:
			result += '*';
			break;
		case QueryToken::DIV:
			result += '/';
			break;
		case QueryToken::MOD:
			result += '%';
			break;
		default:
			throw SyntacticErrorException("Not handled by Expr");
		}

	}
	return result;

}

void Utility::isSyntacticValidpattern(std::vector<QueryToken>token_chain) {
	std::vector<std::vector<QueryToken>> separated_params;
	for (int i = 0; i < 3; i++) {
		separated_params.push_back(std::vector<QueryToken>{});
	}

	int delimiter_count = 0;
	size_t token_chain_size = token_chain.size();

	for (size_t i = 0; i < token_chain_size; i++) {
		if (token_chain[0].type == QueryToken::COMMA) {
			token_chain.erase(token_chain.begin());
			delimiter_count++;
		} else if (delimiter_count >= 0 && delimiter_count < 3) {
			// 1st param
			separated_params[delimiter_count].push_back(token_chain[0]);
			token_chain.erase(token_chain.begin());
		} else {
			throw SyntacticErrorException("Invalid parameters");
		}
	}

	if (delimiter_count == 1) {
		//1st param: entRef
		//2nd param: wildcard | expr
		if (!Utility::isSyntacticValidEntRef(separated_params[0]) ||
			!(Utility::isWildCard(separated_params[1]) || Utility::isExpr(separated_params[1]))) {
			throw SyntacticErrorException("Invalid parameters");
		}

	} else if (delimiter_count == 2) {
		//1st param: entRef
		//2nd & 3rd param: wildcard 
		if (!Utility::isSyntacticValidEntRef(separated_params[0]) ||
			!Utility::isWildCard(separated_params[1]) ||
			!Utility::isWildCard(separated_params[2])) {
			throw SyntacticErrorException("Invalid parameters");
		}

	} else {
		throw SyntacticErrorException("Invalid parameters");
	}

}

bool Utility::isStmtRef(Query& query, std::vector<QueryToken> token_chain) {

	// no args found, throw syntax errors
	if (token_chain.size() == 0) {
		throw SyntacticErrorException("Invalid stmtRef arguments");
	}

	// more than 1 args found. possible entref
	if (token_chain.size() > 1) {
		return false;
	}

	QueryToken token = token_chain[0];

	if (token.type == QueryToken::CONSTANT) {
		if (token.token_value == "0") {
			query.setIsSemanticError("0 is not a valid stmtRef");
		}
		return true;
	}

	if (token.type == QueryToken::WILDCARD) {
		return true;
	}

	// check synonym if is STMT
	if (token.type == QueryToken::IDENTIFIER) {
		std::unordered_map<std::string, Entity> ent_chain = query.getEntities();
		if (ent_chain.find(token.token_value) != ent_chain.end()) {
			return ent_chain.at(token.token_value).getType() == EntityType::STMT ||
				ent_chain.at(token.token_value).getType() == EntityType::READ ||
				ent_chain.at(token.token_value).getType() == EntityType::PRINT ||
				ent_chain.at(token.token_value).getType() == EntityType::CALL ||
				ent_chain.at(token.token_value).getType() == EntityType::PROG_LINE ||
				ent_chain.at(token.token_value).getType() == EntityType::WHILE ||
				ent_chain.at(token.token_value).getType() == EntityType::IF ||
				ent_chain.at(token.token_value).getType() == EntityType::ASSIGN;
		} else {
			// undeclared synonym
			return false;
		}
	}

	// unknown query token
	throw SyntacticErrorException("Invalid stmtRef arguments");
}

bool Utility::isStmtRef(Query& query, std::vector<QueryToken> token_chain, EntityType entity_type) {

	// no args found, throw syntax errors
	if (token_chain.size() == 0) {
		throw SyntacticErrorException("Invalid stmtRef arguments");
	}

	// more than 1 args found. possible entref
	if (token_chain.size() > 1) {
		return false;
	}

	QueryToken token = token_chain[0];

	if (token.type == QueryToken::CONSTANT) {
		return token.token_value != "0";
	}

	if (token.type == QueryToken::WILDCARD) {
		return true;
	}

	// check synonym if is the given entity type
	if (token.type == QueryToken::IDENTIFIER) {
		std::unordered_map<std::string, Entity> ent_chain = query.getEntities();
		if (ent_chain.find(token.token_value) != ent_chain.end()) {
			return ent_chain.at(token.token_value).getType() == entity_type;
		} else {
			// undeclared synonym
			return false;
		}
	}

	// unknown query token
	throw SyntacticErrorException("Invalid stmtRef arguments");
}

bool Utility::isSyntacticValidEntRef(std::vector<QueryToken> token_chain) {
	if (token_chain.size() == 0) {
		return false;
	} else if (token_chain.size() == 1 &&
		(token_chain[0].type == QueryToken::WILDCARD || token_chain[0].type == QueryToken::IDENTIFIER)) {
		return true;
	} else if (token_chain.size() == 3 &&
		token_chain[0].type == QueryToken::QUOTATION_OPEN &&
		token_chain[1].type == QueryToken::IDENTIFIER &&
		token_chain[2].type == QueryToken::QUOTATION_CLOSE) {
		return true;
	} else {
		return false;
	}
}

bool Utility::isSemanticValidEntRef(Query& query, std::vector<QueryToken> token_chain, EntityType entity_type) {
	// entRef : synonym | _ | " IDENT "

	if (token_chain.size() == 1) {
		QueryToken token = token_chain[0];

		if (token.type == QueryToken::WILDCARD) {
			return true;
		} else if (token.type == QueryToken::IDENTIFIER) {
			// check synonym if is EntRef
			std::unordered_map<std::string, Entity> ent_chain = query.getEntities();

			if (ent_chain.find(token.token_value) != ent_chain.end()) {
				return ent_chain.at(token.token_value).getType() == entity_type;
			} else {
				// Undeclared Syn, Cannot find Entity in Query
				return false;
			}
		} else {
			return false;
		}

	} else if (token_chain.size() == 3) {
		//checking for " IDENT " 
		if (token_chain[0].type == QueryToken::QUOTATION_OPEN &&
			token_chain[1].type == QueryToken::IDENTIFIER &&
			token_chain[2].type == QueryToken::QUOTATION_CLOSE) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool Utility::isEntRef(Query& query, std::vector<QueryToken> token_chain, EntityType entity_type) {
	// entRef : synonym | _ | " IDENT "

	if (token_chain.size() == 0) {
		throw SyntacticErrorException("Invalid entRef arguments");
	} else if (token_chain.size() == 1) {
		QueryToken token = token_chain[0];

		if (token.type == QueryToken::WILDCARD) {
			return true;
		} else if (token.type == QueryToken::IDENTIFIER) {
			// check synonym if is EntRef
			std::unordered_map<std::string, Entity> ent_chain = query.getEntities();

			if (ent_chain.find(token.token_value) != ent_chain.end()) {
				return ent_chain.at(token.token_value).getType() == entity_type;
			} else {
				// Undeclared Syn, Cannot find Entity in Query
				return false;
			}
		} else {
			// unknown character
			throw SyntacticErrorException("Invalid entRef arguments");
		}

	} else if (token_chain.size() == 3) {
		//checking for " IDENT " 
		if (token_chain[0].type == QueryToken::QUOTATION_OPEN &&
			token_chain[1].type == QueryToken::IDENTIFIER &&
			token_chain[2].type == QueryToken::QUOTATION_CLOSE) {
			return true;
		} else {
			throw SyntacticErrorException("Invalid EntRef arguments");
		}
	} else {
		throw SyntacticErrorException("Invalid EntRef arguments");
	}
}

bool Utility::isLineRef(Query& query, std::vector<QueryToken> token_chain) {
	// no args found, throw syntax errors
	if (token_chain.size() != 1) {
		throw SyntacticErrorException("Invalid lineRef arguments");
	}

	QueryToken token = token_chain[0];

	if (token.type == QueryToken::CONSTANT) {
		return true;
	} else if (token.type == QueryToken::WILDCARD) {
		return true;
	}
	// check synonym if is program lines
	else if (token.type == QueryToken::IDENTIFIER) {
		std::unordered_map<std::string, Entity> ent_chain = query.getEntities();
		if (ent_chain.find(token.token_value) != ent_chain.end()) {
			return ent_chain.at(token.token_value).getType() == EntityType::STMT ||
				ent_chain.at(token.token_value).getType() == EntityType::READ ||
				ent_chain.at(token.token_value).getType() == EntityType::PRINT ||
				ent_chain.at(token.token_value).getType() == EntityType::CALL ||
				ent_chain.at(token.token_value).getType() == EntityType::PROG_LINE ||
				ent_chain.at(token.token_value).getType() == EntityType::WHILE ||
				ent_chain.at(token.token_value).getType() == EntityType::IF ||
				ent_chain.at(token.token_value).getType() == EntityType::ASSIGN;
		} else {
			// Undeclared Syn, Cannot find Entity in Query
			return false;
		}
	} else {
		// unknown character
		throw SyntacticErrorException("Invalid lineRef arguments");
	}
}

bool Utility::isRef(Query& query, std::vector<QueryToken> token_chain) {
	// entRef : synonym | _ | " IDENT " | synonym.attrName

	std::unordered_map<std::string, Entity> ent_chain = query.getEntities();

	if (token_chain.size() == 0) {
		throw SyntacticErrorException("Invalid Ref arguments");
	} else if (token_chain.size() == 1) {
		QueryToken token = token_chain[0];
		if (token.type == QueryToken::CONSTANT) {
			return true;
		} else if (token.type == QueryToken::IDENTIFIER) {
			// check synonym if is declared

			if (ent_chain.find(token.token_value) != ent_chain.end()) {
				return ent_chain.at(token.token_value).getType() == EntityType::PROG_LINE;
			} else {
				// Undeclared Syn, Cannot find Entity in Query
				return false;
			}
		} else {
			throw SyntacticErrorException("Invalid Ref arguments");
		}

	} else if (token_chain.size() == 3) {
		//checking for " IDENT " 
		if (token_chain[0].type == QueryToken::QUOTATION_OPEN &&
			token_chain[1].type == QueryToken::IDENTIFIER &&
			token_chain[2].type == QueryToken::QUOTATION_CLOSE) {
			return true;
		} else if (token_chain[0].type != QueryToken::IDENTIFIER) {
			throw SyntacticErrorException("Invalid AttrRef");

		} else if (ent_chain.find(token_chain[0].token_value) == ent_chain.end()) {
			return false;
		} else if (token_chain[0].type == QueryToken::IDENTIFIER &&
			token_chain[1].type == QueryToken::DOT &&
			token_chain[2].type == QueryToken::VAR_NAME &&
			(ent_chain.at(token_chain[0].token_value).getType() == EntityType::VARIABLE ||
				ent_chain.at(token_chain[0].token_value).getType() == EntityType::READ ||
				ent_chain.at(token_chain[0].token_value).getType() == EntityType::PRINT)) {

			return true;
		} else if (token_chain[0].type == QueryToken::IDENTIFIER &&
			token_chain[1].type == QueryToken::DOT &&
			token_chain[2].type == QueryToken::PROC_NAME &&
			(ent_chain.at(token_chain[0].token_value).getType() == EntityType::CALL ||
				ent_chain.at(token_chain[0].token_value).getType() == EntityType::PROCEDURE)) {

			return true;
		} else if (token_chain[0].type == QueryToken::IDENTIFIER &&
			token_chain[1].type == QueryToken::DOT &&
			token_chain[2].type == QueryToken::STMT_INDEX &&
			(ent_chain.at(token_chain[0].token_value).getType() == EntityType::STMT ||
				ent_chain.at(token_chain[0].token_value).getType() == EntityType::READ ||
				ent_chain.at(token_chain[0].token_value).getType() == EntityType::PRINT ||
				ent_chain.at(token_chain[0].token_value).getType() == EntityType::CALL ||
				ent_chain.at(token_chain[0].token_value).getType() == EntityType::WHILE ||
				ent_chain.at(token_chain[0].token_value).getType() == EntityType::IF ||
				ent_chain.at(token_chain[0].token_value).getType() == EntityType::ASSIGN)) {

			return true;
		} else if (token_chain[0].type == QueryToken::IDENTIFIER &&
			token_chain[1].type == QueryToken::DOT &&
			token_chain[2].type == QueryToken::VALUE &&
			ent_chain.at(token_chain[0].token_value).getType() == EntityType::CONSTANT) {

			return true;
		} else {
			return false;
		}
	} else {
		throw SyntacticErrorException("Invalid ref arguments");
	}
}

bool Utility::isExpr(std::vector<QueryToken> token_chain) {

	size_t token_chain_size = token_chain.size();

	if (token_chain_size == 0) {
		throw SyntacticErrorException("Invalid expr arguments");

	} else if (token_chain_size == 1) {

		return token_chain[0].type == QueryToken::WILDCARD;

	} else if (token_chain_size != 2 &&
		token_chain[0].type == QueryToken::QUOTATION_OPEN &&
		token_chain[token_chain_size - 1].type == QueryToken::QUOTATION_CLOSE) {

		return true;

	} else if (token_chain_size != 4 &&
		token_chain[0].type == QueryToken::WILDCARD &&
		token_chain[1].type == QueryToken::QUOTATION_OPEN &&
		token_chain[token_chain_size - 2].type == QueryToken::QUOTATION_CLOSE &&
		token_chain[token_chain_size - 1].type == QueryToken::WILDCARD) {

		return true;

	} else {
		throw SyntacticErrorException("Invalid expr arguments");
	}
}

bool Utility::isWildCard(std::vector<QueryToken> token_chain) {
	if (token_chain.size() == 1 &&
		token_chain[0].type == QueryToken::WILDCARD) {
		return true;
	} else {
		return false;
	}
}

bool Utility::isStringRefType(Query& query, std::vector<QueryToken> token_chain) {

	std::unordered_map<std::string, Entity> ent_chain = query.getEntities();
	if (token_chain.size() == 3) {
		//checking for " IDENT " 
		if (token_chain[0].type == QueryToken::QUOTATION_OPEN &&
			token_chain[1].type == QueryToken::IDENTIFIER &&
			token_chain[2].type == QueryToken::QUOTATION_CLOSE) {
			return true;
		} else if (token_chain[0].type == QueryToken::IDENTIFIER &&
			token_chain[1].type == QueryToken::DOT &&
			token_chain[2].type == QueryToken::VAR_NAME &&
			ent_chain.at(token_chain[0].token_value).getType() == EntityType::VARIABLE ||
			ent_chain.at(token_chain[0].token_value).getType() == EntityType::READ ||
			ent_chain.at(token_chain[0].token_value).getType() == EntityType::PRINT) {

			return true;
		} else if (token_chain[0].type == QueryToken::IDENTIFIER &&
			token_chain[1].type == QueryToken::DOT &&
			token_chain[2].type == QueryToken::PROC_NAME &&
			ent_chain.at(token_chain[0].token_value).getType() == EntityType::CALL ||
			ent_chain.at(token_chain[0].token_value).getType() == EntityType::PROCEDURE) {

			return true;
		} else if (token_chain[0].type == QueryToken::IDENTIFIER &&
			token_chain[1].type == QueryToken::DOT &&
			token_chain[2].type == QueryToken::STMT_INDEX ||
			token_chain[2].type == QueryToken::VALUE) {
			return false;
		} else {
			query.setIsSemanticError("ref has no string value");
		}
	} else {
		return false;
	}
}

bool Utility::isIntRefType(Query& query, std::vector<QueryToken> token_chain) {
	std::unordered_map<std::string, Entity> ent_chain = query.getEntities();
	if (token_chain.size() == 1) {
		if (token_chain[0].type == QueryToken::CONSTANT) {
			return true;
		} else if (token_chain[0].type == QueryToken::IDENTIFIER &&
			ent_chain.at(token_chain[0].token_value).getType() == EntityType::PROG_LINE) {

			return true;
		}
	} else if (token_chain.size() == 3) {
		if (token_chain[0].type != QueryToken::IDENTIFIER &&
			token_chain[1].type != QueryToken::DOT) {
			return false;
		}
		if (token_chain[2].type == QueryToken::STMT_INDEX &&
			(ent_chain.at(token_chain[0].token_value).getType() == EntityType::STMT ||
				ent_chain.at(token_chain[0].token_value).getType() == EntityType::READ ||
				ent_chain.at(token_chain[0].token_value).getType() == EntityType::PRINT ||
				ent_chain.at(token_chain[0].token_value).getType() == EntityType::CALL ||
				ent_chain.at(token_chain[0].token_value).getType() == EntityType::WHILE ||
				ent_chain.at(token_chain[0].token_value).getType() == EntityType::IF ||
				ent_chain.at(token_chain[0].token_value).getType() == EntityType::ASSIGN)) {

			return true;
		} else if (token_chain[2].type == QueryToken::VALUE &&
			ent_chain.at(token_chain[0].token_value).getType() == EntityType::CONSTANT) {

			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

Entity Utility::setStmtRef(Query& query, QueryToken token, EntityType ident_type) {
	// synonym | _ | INTEGER

	// wild card check
	if (token.type == QueryToken::WILDCARD) {
		return Entity(EntityType::WILD);
	}

	// is INTEGER, constant type
	if (token.type == QueryToken::CONSTANT) {
		return Entity(ident_type, token.token_value);
	}

	// synonym check
	std::unordered_map<std::string, Entity> ent_chain = query.getEntities();
	if (ent_chain.find(token.token_value) != ent_chain.end()) {
		return ent_chain.at(token.token_value);
	}

	query.setIsSemanticError("Unknown stmtRef");
}

Entity Utility::setEntRef(Query& query,
	std::vector<QueryToken> token_chain, EntityType ident_type) {
	// entRef : synonym | _ | "IDENT"

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
	// IDENT : LETTER ( LETTER | DIGIT )*
	if (token_chain.size() == 3) {
		return Entity(ident_type, token_chain[1].token_value);
	}


	query.setIsSemanticError("Unknown entRef");
}

Entity Utility::setLineRef(Query& query, QueryToken token) {
	// synonym | "_" | INTEGER

	// wild card check
	if (token.type == QueryToken::WILDCARD) {
		return Entity(EntityType::WILD);
	}

	// is INTEGER, constant type
	if (token.type == QueryToken::CONSTANT) {
		return Entity(EntityType::PROG_LINE, token.token_value);
	}

	// synonym check
	std::unordered_map<std::string, Entity> ent_chain = query.getEntities();
	if (ent_chain.find(token.token_value) != ent_chain.end()) {

		return ent_chain.at(token.token_value);
	}

	query.setIsSemanticError("Unknown lineRef");
}

Entity Utility::setRef(Query& query,
	std::vector<QueryToken> token_chain, EntityType ident_type, AttrRef attr_name) {
	std::unordered_map<std::string, Entity> ent_chain = query.getEntities();

	if (token_chain.size() == 1) {
		if (token_chain[0].type == QueryToken::CONSTANT && attr_name == AttrRef::NONE) {
			return Entity(EntityType::BOOLEAN, token_chain[0].token_value);
		} else if (token_chain[0].type == QueryToken::CONSTANT) {
			return Entity(ident_type, token_chain[0].token_value);
		} else
			// synonym check
			std::unordered_map<std::string, Entity> ent_chain = query.getEntities();
		if (ent_chain.find(token_chain[0].token_value) != ent_chain.end()) {
			return ent_chain.at(token_chain[0].token_value);
		}
	} else if (token_chain.size() == 3) {
		if (token_chain[0].type == QueryToken::QUOTATION_OPEN &&
			token_chain[1].type == QueryToken::IDENTIFIER &&
			token_chain[2].type == QueryToken::QUOTATION_CLOSE) {
			Entity ent = Entity(ident_type, token_chain[1].token_value);
			ent.setAttribute(attr_name);
			return ent;
		} else if (token_chain[2].type == QueryToken::PROC_NAME) {
			Entity ent = ent_chain.at(token_chain[0].token_value);
			ent.setAttribute(AttrRef::PROC_NAME);
			return ent;
		} else if (token_chain[2].type == QueryToken::VAR_NAME) {
			Entity ent = ent_chain.at(token_chain[0].token_value);
			ent.setAttribute(AttrRef::VAR_NAME);
			return ent;
		} else if (token_chain[2].type == QueryToken::VALUE) {
			Entity ent = ent_chain.at(token_chain[0].token_value);
			ent.setAttribute(AttrRef::VALUE);
			return ent;
		} else if (token_chain[2].type == QueryToken::STMT_INDEX) {
			Entity ent = ent_chain.at(token_chain[0].token_value);
			ent.setAttribute(AttrRef::STMT_INDEX);
			return ent;
		}
	}
	query.setIsSemanticError("Unknown Ref");
}

std::string Utility::setExpr(std::vector<QueryToken> token_chain) {
	// expression-spec : "expr" | _"expr"_ | _
	int wildcard_count = 0;
	bool is_quotation_open = false;
	bool is_quotation_close = false;
	std::string result = "";
	if (token_chain.size() == 1 && token_chain[0].type == QueryToken::WILDCARD) {
		return result;
	} else {
		if (token_chain[0].type == QueryToken::WILDCARD) {
			token_chain.erase(token_chain.begin() + token_chain.size() - 1);
			token_chain.erase(token_chain.begin());
		}
		if (token_chain[0].type == QueryToken::QUOTATION_OPEN) {
			token_chain.erase(token_chain.begin() + token_chain.size() - 1);
			token_chain.erase(token_chain.begin());
		}
	}
	return Utility::queryTokenTypeToExprString(token_chain);
}

std::vector<std::vector<QueryToken>> Utility::splitTokenChain(int max_params, QueryToken::QueryTokenType delimiter, std::vector<QueryToken> token_chain) {
	std::vector<std::vector<QueryToken>> separated_params;
	for (int i = 0; i < max_params; i++) {
		separated_params.push_back(std::vector<QueryToken>{});
	}

	int delimiter_count = 0;
	size_t token_chain_size = token_chain.size();
	for (size_t i = 0; i < token_chain_size; i++) {
		if (token_chain[0].type == delimiter) {
			token_chain.erase(token_chain.begin());
			delimiter_count++;
		} else if (delimiter_count >= 0 && delimiter_count < max_params) {
			// 1st param
			separated_params[delimiter_count].push_back(token_chain[0]);
			token_chain.erase(token_chain.begin());
		} else {
			throw SyntacticErrorException("Invalid parameters");
		}
	}
	return separated_params;
}

bool Utility::checkIsSemanticError(Query& query) {
	return query.getIsSemanticError() != "";
}

bool Utility::isStringEntityType(EntityType e) {
	return e == EntityType::PROCEDURE || e == EntityType::VARIABLE;
}

bool Utility::isStmt(EntityType e) {
	return e == EntityType::STMT || e == EntityType::PROG_LINE;
}

value Utility::hashString(std::string to_hash, std::unordered_map<value, std::string>& hash_storage) {
	value hash = std::hash<std::string>{}(to_hash);
	hash_storage.insert({ hash, to_hash });
	return hash;
}


#include <algorithm>
#include <sstream>

#include "Utility.h"


std::list<std::string> Utility::constantsToStringList(std::vector<constant>& from) {
	std::list<std::string> to;
	for (auto& it : from) {
		to.push_back(std::to_string(it));
	}
	return to;
}

std::list<std::string> Utility::proceduresToStringList(std::vector<proc_name>& from) {
	std::list<std::string> to;
	for (auto& it : from) {
		to.push_back(it.c_str());
	}
	return to;
}

std::list<std::string> Utility::stmtInfoToStringList(std::vector<StmtInfo>& from) {
	std::list<std::string> to;
	for (auto& it : from) {
		to.push_back(std::to_string(it.stmt_index));
	}
	return to;
}
std::list<std::string> Utility::stmtIndexToStringList(std::vector<stmt_index>& from) {
	std::list<std::string> to;
	for (auto& it : from) {
		to.push_back(std::to_string(it));
	}
	return to;
}

std::vector<std::pair<std::string, std::string>> Utility::stringListToStringPair(std::list<std::string>& from) {
	std::vector<std::pair<std::string, std::string>> to;
	for (auto& it : from) {
		to.push_back({ it, it });
	}
	return to;
}


std::list<std::vector<std::string>> Utility::pairToStringTable(std::vector<std::pair<std::string, std::string>>& from) {
	std::list< std::vector<std::string>> to;
	for (auto& it : from) {
		to.push_back({ it.first, it.second });
	}
	return to;
}

std::list<std::vector<std::string>> Utility::pairToStringTable(std::vector<std::pair<stmt_index, std::string>>& from) {
	std::list< std::vector<std::string>> to;
	for (auto& it : from) {
		to.push_back({ std::to_string(it.first), it.second });
	}
	return to;
}

std::list<std::vector<std::string>> Utility::pairToStringTable(std::vector<std::pair<StmtInfo, std::string>>& from) {
	std::list< std::vector<std::string>> to;
	for (auto& it : from) {
		to.push_back({ std::to_string(it.first.stmt_index), it.second });
	}
	return to;
}

std::list<std::vector<std::string>> Utility::pairToStringTable(std::vector<std::pair<StmtInfo, StmtInfo>>& from) {
	std::list< std::vector<std::string>> to;
	for (auto& it : from) {
		to.push_back({ std::to_string(it.first.stmt_index), std::to_string(it.second.stmt_index) });
	}
	return to;
}

std::list<std::string> Utility::variablesToStringList(std::vector<var_name>& from) {
	std::list<std::string> to;
	for (auto& it : from) {
		to.push_back(it.c_str());
	}
	return to;
}

std::list<std::string> Utility::unorderedSetToStringList(std::unordered_set<std::string>& from) {
	return std::list<std::string>(from.begin(), from.end());
}

int Utility::getIndex(std::vector<std::string> v, std::string s) {
	auto it = std::find(v.begin(), v.end(), s);
	if (it != v.end()) {
		int index = it - v.begin();
		return index;
	}
	else {
		return -1;
	}
}

int Utility::getIndex(std::vector<Entity> v, Entity s) {
	auto it = std::find(v.begin(), v.end(), s);
	if (it != v.end()) {
		int index = it - v.begin();
		return index;
	}
	else {
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
	default: throw std::invalid_argument("convert(): only does not support this type " + e);
	}

	return StmtType();
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

std::list<std::vector<std::string>> Utility::filterResults(EntityType e, 
	std::vector<std::pair<StmtInfo, std::string>>& v) {
	if (isStmt(e)) {
		return pairToStringTable(v);
	}

	StmtType type = convertType(e);;
	std::vector<std::pair<StmtInfo, std::string>> result;
	for (auto& it : v) {
		if (it.first.stmt_type == type) {
			result.push_back(it);
		}
	}
	return pairToStringTable(result);
}

std::list<std::vector<std::string>> Utility::filterResults(
	std::pair<EntityType, EntityType> type, std::vector<std::pair<StmtInfo, StmtInfo>>& table) {
	if (isStmt(type.first) && isStmt(type.second)) {
		return pairToStringTable(table);
	}

	std::vector < std::pair<StmtInfo, StmtInfo>> results;
	StmtType type1 = convertType(type.first);
	StmtType type2 = convertType(type.second);
	for (auto& row : table) {
		if ((isStmt(type.first) || type1 == row.first.stmt_type) &&
			(isStmt(type.second) || type2 == row.second.stmt_type)) {
			results.push_back(row);
		}
	}

	return pairToStringTable(results);
}

std::list<std::vector<std::string>> Utility::filterResults(
	std::list<std::vector<std::string>>& from, std::unordered_set<std::string>& filter, int index) {
	std::list<std::vector<std::string>> results;

	for (auto& it : from) {
		if (filter.find(it[index]) != filter.end()) {
			results.push_back(it);
		}
	}

	return results;
}

std::list<std::vector<std::string>> Utility::filterResults(std::list<std::vector<std::string>>& from,
	std::unordered_map<std::string, std::unordered_set<std::string>>& filter, int index1, int index2) {
	std::list<std::vector<std::string>> results;

	for (auto& v : from) {
		auto it1 = filter.find(v[index1]);
		if (it1 != filter.end()) {
			if (it1->second.find(v[index2]) != it1->second.end()) {
				results.push_back(v);
			}
		}
	}

	return results;
}

std::list<std::string> Utility::mergeColumnEqual(std::list<std::vector<std::string>>& v) {
	std::list<std::string> to;
	for (auto& row : v) {
		bool equal = true;
		for (unsigned int j = 1; j < row.size(); j++) {
			if (row[j - 1] != row[j]) {
				equal = false;
				break;
			}
		}
		if (equal) {
			to.push_back(row[0]);
		}
	}
	return to;
}

std::list<std::vector<std::string>> Utility::joinTable(std::list<std::vector<std::string>>& main, int main_header_index,
	std::unordered_multimap<std::string, std::vector<std::string>>& to_join, int to_join_header_index) {
	std::list<std::vector<std::string>> results;

	for (auto& it : main) {
		if (to_join.count(it[main_header_index]) == 0) {
			continue;
		}
		std::pair<std::unordered_multimap<std::string, std::vector<std::string>>::iterator, 
			std::unordered_multimap<std::string, std::vector<std::string>>::iterator> ret;

		ret = to_join.equal_range(it[main_header_index]);

		for (std::unordered_multimap<std::string, std::vector<std::string>>::iterator itr1 = ret.first; itr1 != ret.second; ++itr1) {
			results.emplace_back(joinRow(it, itr1->second, to_join_header_index));
		}
	}
	return results;
}

std::vector<std::string> Utility::joinRow(std::vector<std::string>& main, std::vector<std::string>& to_join) {
	std::vector<std::string> result(main);
	for (unsigned int i = 0; i < to_join.size(); i++) {
		result.push_back(to_join[i]);
	}
	return result;
}

std::vector<std::string> Utility::joinRow(std::vector<std::string>& main, std::vector<std::string>& to_join, int common_index) {
	std::vector<std::string> result(main);
	for (unsigned int i = 0; i < to_join.size(); i++) {
		if (i == common_index) {
			continue;
		}
		result.push_back(to_join[i]);
	}
	return result;
}


std::list<std::vector<std::string>> Utility::joinTable(std::list<std::vector<std::string>>& main, std::list<std::vector<std::string>>& to_join) {
	std::list<std::vector<std::string>> result;
	for (auto& main_row : main) {
		for (auto& to_join_row : to_join) {
			result.push_back(joinRow(main_row, to_join_row));
		}
	}
	return result;
}

std::vector<Entity> Utility::removeEntities(std::vector<Entity>& main, std::vector<Entity>& to_remove) {

	std::unordered_set<std::string> to_remove_set;
	for (auto& e : to_remove) {
		to_remove_set.insert(e.getSynonym());
	}

	std::vector<Entity> result;
	
	for (auto& e : main) {
		if (to_remove_set.count(e.getSynonym()) == 0) {
			result.push_back(e);
		}
	}

	return result;
}

std::vector<Entity> Utility::removeDuplicateEntities(std::vector<Entity>& main) {
	std::unordered_set<std::string> unique;
	std::vector<Entity> result;

	for (auto& e : main) {
		if (unique.count(e.getSynonym()) == 0) {
			result.push_back(e);
			unique.insert(e.getSynonym());
		}
	}
	return result;
}

std::list<std::vector<std::string>> Utility::getColumnsNoDuplicate(std::list<std::vector<std::string>>& main, std::vector<int>& indexes) {
	std::unordered_set<std::string> unique;
	std::list<std::vector<std::string>> result;
	for (auto& row : main) {
		std::vector<std::string> newRow;
		std::string rowString;
		for (int i = 0; i < indexes.size(); i++) {
			newRow.emplace_back(row[indexes[i]]);
			rowString += row[indexes[i]] + " ";
		}
		if (unique.count(rowString) == 0) {
			result.emplace_back(newRow);
			unique.insert(rowString);
		}
	}
	return result;
}

bool Utility::isSecondaryAttribute(Entity e) {
	return (e.getType() == EntityType::CALL && e.getAttribute() == AttrRef::PROC_NAME) ||
		(e.getType() == EntityType::READ && e.getAttribute() == AttrRef::VAR_NAME) ||
		(e.getType() == EntityType::PRINT && e.getAttribute() == AttrRef::VAR_NAME);
}


EntityType Utility::queryTokenTypeToEntityType(QueryToken::QueryTokenType& query_token_type) {
	if (query_token_type == QueryToken::QueryTokenType::STMT) {
		return EntityType::STMT;
	}
	else if (query_token_type == QueryToken::QueryTokenType::PROCEDURE) {
		return EntityType::PROCEDURE;
	}
	else if (query_token_type == QueryToken::QueryTokenType::READ) {
		return EntityType::READ;
	}
	else if (query_token_type == QueryToken::QueryTokenType::PRINT) {
		return EntityType::PRINT;
	}
	else if (query_token_type == QueryToken::QueryTokenType::CALL) {
		return EntityType::CALL;
	}
	else if (query_token_type == QueryToken::QueryTokenType::IF) {
		return EntityType::IF;
	}
	else if (query_token_type == QueryToken::QueryTokenType::WHILE) {
		return EntityType::WHILE;
	}
	else if (query_token_type == QueryToken::QueryTokenType::ASSIGN) {
		return EntityType::ASSIGN;
	}
	else if (query_token_type == QueryToken::QueryTokenType::VARIABLE) {
		return EntityType::VARIABLE;
	}
	else if (query_token_type == QueryToken::QueryTokenType::CONSTANT) {
		return EntityType::CONSTANT;
	}
	else if (query_token_type == QueryToken::QueryTokenType::PROG_LINE) {
		return EntityType::PROG_LINE;
	}
}

AttrRef Utility::queryTokenTypeToAttrRef(QueryToken::QueryTokenType& query_token_type) {
	if (query_token_type == QueryToken::QueryTokenType::PROC_NAME) {
		return AttrRef::PROC_NAME;
	}
	else if (query_token_type == QueryToken::QueryTokenType::VAR_NAME) {
		return AttrRef::VAR_NAME;
	}
	else if (query_token_type == QueryToken::QueryTokenType::VALUE) {
		return AttrRef::VALUE;
	}
	else if (query_token_type == QueryToken::QueryTokenType::STMT_INDEX) {
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
		}
		else {
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
		return true;
	}

	if (token.type == QueryToken::WILDCARD) {
		return true;
	}

	// check synonym if is the given entity type
	if (token.type == QueryToken::IDENTIFIER) {
		std::unordered_map<std::string, Entity> ent_chain = query.getEntities();
		if (ent_chain.find(token.token_value) != ent_chain.end()) {
			return ent_chain.at(token.token_value).getType() == entity_type;
		}
		else {
			// undeclared synonym
			return false;
		}
	}

	// unknown query token
	throw SyntacticErrorException("Invalid stmtRef arguments");
}


bool Utility::isEntRef(Query& query, std::vector<QueryToken> token_chain) {
	// entRef : synonym | ‘_’ | ‘"’ IDENT ‘"’

	if (token_chain.size() == 0) {
		throw SyntacticErrorException("Invalid entRef arguments");
	}
	else if (token_chain.size() == 1) {
		QueryToken token = token_chain[0];

		if (token.type == QueryToken::WILDCARD) {
			return true;
		}
		else if (token.type == QueryToken::IDENTIFIER) {
			// check synonym if is EntRef
			std::unordered_map<std::string, Entity> ent_chain = query.getEntities();

			if (ent_chain.find(token.token_value) != ent_chain.end()) {
				return ent_chain.at(token.token_value).getType() == EntityType::VARIABLE ||
					ent_chain.at(token.token_value).getType() == EntityType::PROCEDURE;
			}
			else {
				// Undeclared Syn, Cannot find Entity in Query
				return false;
			}
		}
		else {
			// unknown character
			throw SyntacticErrorException("Invalid entRef arguments");
		}

	}
	else if (token_chain.size() == 3) {
		//checking for " IDENT " 
		if (token_chain[0].type == QueryToken::QUOTATION_OPEN &&
			token_chain[1].type == QueryToken::IDENTIFIER &&
			token_chain[2].type == QueryToken::QUOTATION_CLOSE) {
			return true;
		}
		else {
			throw SyntacticErrorException("Invalid EntRef arguments");
		}
	}
	else {
		throw SyntacticErrorException("Invalid EntRef arguments");
	}
}

bool Utility::isLineRef(Query& query, std::vector<QueryToken> token_chain) {

	// no args found, throw syntax errors
	if (token_chain.size() != 1) {
		throw SyntacticErrorException("Invalid lineRef arguments");
	}

	QueryToken token = token_chain[0];

	//if integer, return true
	if (token.type == QueryToken::CONSTANT) {
		return true;
	}
	else if (token.type == QueryToken::WILDCARD) {
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
		}
		else {
			// Undeclared Syn, Cannot find Entity in Query
			return false;
		}
	}
	else {
		// unknown character
		throw SyntacticErrorException("Invalid lineRef arguments");
	}
}

bool Utility::isRef(Query& query, std::vector<QueryToken> token_chain) {
	// entRef : synonym | ‘_’ | ‘"’ IDENT ‘"’ | synonym"."attrName
	
	std::unordered_map<std::string, Entity> ent_chain = query.getEntities();

	if (token_chain.size() == 0) {
		throw SyntacticErrorException("Invalid Ref arguments");
	}
	else if (token_chain.size() == 1) {
		QueryToken token = token_chain[0];
		if (token.type == QueryToken::CONSTANT) {
			return true;
		}
		else if (token.type == QueryToken::IDENTIFIER) {
			// check synonym if is declared

			if (ent_chain.find(token.token_value) != ent_chain.end()) {
				return ent_chain.at(token.token_value).getType() == EntityType::PROG_LINE;
			}
			else {
				// Undeclared Syn, Cannot find Entity in Query
				return false;
			}
		}
		else {
			throw SyntacticErrorException("Invalid Ref arguments");
		}

	}
	else if (token_chain.size() == 3) {
		//checking for " IDENT " 
		if (token_chain[0].type == QueryToken::QUOTATION_OPEN &&
			token_chain[1].type == QueryToken::IDENTIFIER &&
			token_chain[2].type == QueryToken::QUOTATION_CLOSE) {
			return true;
		}
		else if (token_chain[0].type != QueryToken::IDENTIFIER) {
			throw SyntacticErrorException("Invalid AttrRef");

		}
		else if (ent_chain.find(token_chain[0].token_value) == ent_chain.end()) {
			return false;
		}
		else if (token_chain[0].type == QueryToken::IDENTIFIER &&
			token_chain[1].type == QueryToken::DOT &&
			token_chain[2].type == QueryToken::VAR_NAME &&
			(ent_chain.at(token_chain[0].token_value).getType() == EntityType::VARIABLE ||
			ent_chain.at(token_chain[0].token_value).getType() == EntityType::READ ||
			ent_chain.at(token_chain[0].token_value).getType() == EntityType::PRINT)) {

			return true;
		}
		else if (token_chain[0].type == QueryToken::IDENTIFIER &&
			token_chain[1].type == QueryToken::DOT &&
			token_chain[2].type == QueryToken::PROC_NAME &&
			(ent_chain.at(token_chain[0].token_value).getType() == EntityType::CALL ||
			ent_chain.at(token_chain[0].token_value).getType() == EntityType::PROCEDURE)) {

			return true;
		}
		else if (token_chain[0].type == QueryToken::IDENTIFIER &&
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
		}
		else if (token_chain[0].type == QueryToken::IDENTIFIER &&
			token_chain[1].type == QueryToken::DOT &&
			token_chain[2].type == QueryToken::VALUE &&
			ent_chain.at(token_chain[0].token_value).getType() == EntityType::CONSTANT) {

			return true;
		}
		else {
			return false;
		}
	}
	else {
		throw SyntacticErrorException("Invalid ref arguments");
	}
}

bool Utility::isExpr(std::vector<QueryToken> token_chain) {

	size_t token_chain_size = token_chain.size();

	if (token_chain_size == 0) {
		throw SyntacticErrorException("Invalid expr arguments");

	}
	else if (token_chain_size == 1) {

		return token_chain[0].type == QueryToken::WILDCARD;

	}
	else if (token_chain_size != 2 &&
		token_chain[0].type == QueryToken::QUOTATION_OPEN &&
		token_chain[token_chain_size - 1].type == QueryToken::QUOTATION_CLOSE) {

		return true;

	}
	else if (token_chain_size != 4 &&
		token_chain[0].type == QueryToken::WILDCARD &&
		token_chain[1].type == QueryToken::QUOTATION_OPEN &&
		token_chain[token_chain_size - 2].type == QueryToken::QUOTATION_CLOSE &&
		token_chain[token_chain_size - 1].type == QueryToken::WILDCARD) {

		return true;

	}
	else {
		throw SyntacticErrorException("Invalid expr arguments");
	}
}

bool Utility::isCorrectSynEntRef(Query& query, std::vector<QueryToken> token_chain,
	EntityType expected_ent_type) {

	std::unordered_map<std::string, Entity> ent_chain = query.getEntities();

	if (token_chain.size() == 1 && token_chain[0].type == QueryToken::IDENTIFIER) {

		QueryToken token = token_chain[0];
		if (ent_chain.find(token.token_value) != ent_chain.end()) {
			return ent_chain.at(token.token_value).getType() == expected_ent_type;
		}

	}
	else {
		return true;
	}
}

bool Utility::isWildCard(std::vector<QueryToken> token_chain) {
	if (token_chain.size() == 0) {
		throw SyntacticErrorException("Invalid arguments, wildcards only");
	}
	else if (token_chain.size() > 1) {
		throw SyntacticErrorException("Invalid arguments, wildcards only");
	}
	else if (token_chain[0].type == QueryToken::WILDCARD) {
		return true;
	}
	else {
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
		}
		else if (token_chain[0].type == QueryToken::IDENTIFIER &&
			token_chain[1].type == QueryToken::DOT &&
			token_chain[2].type == QueryToken::VAR_NAME && 
			ent_chain.at(token_chain[0].token_value).getType() == EntityType::VARIABLE ||
			ent_chain.at(token_chain[0].token_value).getType() == EntityType::READ ||
			ent_chain.at(token_chain[0].token_value).getType() == EntityType::PRINT) {

			return true;
		}
		else if (token_chain[0].type == QueryToken::IDENTIFIER &&
			token_chain[1].type == QueryToken::DOT &&
			token_chain[2].type == QueryToken::PROC_NAME && 
			ent_chain.at(token_chain[0].token_value).getType() == EntityType::CALL ||
			ent_chain.at(token_chain[0].token_value).getType() == EntityType::PROCEDURE) {

			return true;
		} 
		else if (token_chain[0].type == QueryToken::IDENTIFIER &&
			token_chain[1].type == QueryToken::DOT &&
			token_chain[2].type == QueryToken::STMT_INDEX ||
			token_chain[2].type == QueryToken::VALUE) {
			return false;
		}
		else {
			query.setIsSemanticError("ref has no string value");
		}
	}
	else {
		return false;
	}
}

bool Utility::isIntRefType(Query& query, std::vector<QueryToken> token_chain) {
	std::unordered_map<std::string, Entity> ent_chain = query.getEntities();
	if (token_chain.size() == 1) {
		if (token_chain[0].type == QueryToken::CONSTANT) {
			return true;
		}
		else if (token_chain[0].type == QueryToken::IDENTIFIER &&
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
		}
		else if (token_chain[2].type == QueryToken::VALUE &&
			ent_chain.at(token_chain[0].token_value).getType() == EntityType::CONSTANT) {

			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

Entity Utility::setStmtRef(Query& query, QueryToken token, EntityType ident_type) {
	// synonym | ‘_’ | INTEGER

	// wild card check
	if (token.type == QueryToken::WILDCARD) {
		return Entity(EntityType::WILD);
	}

	// is INTEGER, constant type
	if (token.type == QueryToken::CONSTANT) {
		return Entity(EntityType::STMT, token.token_value);
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
	// IDENT : LETTER ( LETTER | DIGIT )*
	if (token_chain.size() == 3) {
		return Entity(ident_type, token_chain[1].token_value);
	}


	query.setIsSemanticError("Unknown entRef");
}

Entity Utility::setLineRef(Query& query, QueryToken token) {
	// synonym | ‘_’ | INTEGER

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
		if (token_chain[0].type == QueryToken::CONSTANT && attr_name ==AttrRef::NONE) {
			return Entity(EntityType::BOOLEAN, token_chain[0].token_value);
		}else if (token_chain[0].type == QueryToken::CONSTANT) {
			return Entity(ident_type, token_chain[0].token_value);
		}
		else
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
		}
		else if (token_chain[2].type == QueryToken::PROC_NAME) {
			Entity ent = ent_chain.at(token_chain[0].token_value);
			ent.setAttribute(AttrRef::PROC_NAME);
			return ent;
		}
		else if (token_chain[2].type == QueryToken::VAR_NAME) {
			Entity ent = ent_chain.at(token_chain[0].token_value);
			ent.setAttribute(AttrRef::VAR_NAME);
			return ent;
		}
		else if (token_chain[2].type == QueryToken::VALUE) {
			Entity ent = ent_chain.at(token_chain[0].token_value);
			ent.setAttribute(AttrRef::VALUE);
			return ent;
		}
		else if (token_chain[2].type == QueryToken::STMT_INDEX) {
			Entity ent = ent_chain.at(token_chain[0].token_value);
			ent.setAttribute(AttrRef::STMT_INDEX);
			return ent;
		}
	}


	query.setIsSemanticError("Unknown Ref");
}

std::string Utility::setExpr(std::vector<QueryToken> token_chain) {
	// expression-spec : ‘"‘ expr’"’ | ‘_’ ‘"’ expr ‘"’ ‘_’ | ‘_’
	int wildcard_count = 0;
	bool is_quotation_open = false;
	bool is_quotation_close = false;
	std::string result = "";
	if (token_chain.size() == 1 && token_chain[0].type == QueryToken::WILDCARD) {
		return result;
	}
	else {
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

bool Utility::checkIsSemanticError(Query& query) {
	return query.getIsSemanticError() != "";
}

bool Utility::Utility::isStmt(EntityType e) {
	return e == EntityType::STMT || e == EntityType::PROG_LINE;
}



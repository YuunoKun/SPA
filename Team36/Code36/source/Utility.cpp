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

std::vector<std::string> Utility::stmtInfoToStringVector(std::vector<StmtInfo>& from) {
	std::vector<std::string> to;
	for (auto& it : from) {
		to.push_back(std::to_string(it.stmt_index));
	}
	return to;
}

std::vector<std::string> Utility::stmtIndexToStringVector(std::vector<stmt_index>& from) {
	std::vector<std::string> to;
	for (auto& it : from) {
		to.push_back(std::to_string(it));
	}
	return to;
}


std::vector<std::vector<std::string>> Utility::pairToStringTable(std::vector<std::pair<std::string, std::string>>& from) {
	std::vector< std::vector<std::string>> to;
	for (auto& it : from) {
		to.push_back({ it.first, it.second });
	}
	return to;
}

std::vector<std::vector<std::string>> Utility::pairToStringTable(std::vector<std::pair<stmt_index, std::string>>& from) {
	std::vector< std::vector<std::string>> to;
	for (auto& it : from) {
		to.push_back({ std::to_string(it.first), it.second });
	}
	return to;
}

std::vector<std::vector<std::string>> Utility::pairToStringTable(std::vector<std::pair<StmtInfo, std::string>>& from) {
	std::vector< std::vector<std::string>> to;
	for (auto& it : from) {
		to.push_back({ std::to_string(it.first.stmt_index), it.second });
	}
	return to;
}

std::vector<std::vector<std::string>> Utility::pairToStringTable(std::vector<std::pair<StmtInfo, StmtInfo>>& from) {
	std::vector< std::vector<std::string>> to;
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
	if (e == EntityType::STMT) {
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

std::vector<std::vector<std::string>> Utility::filterResults(EntityType e, 
	std::vector<std::pair<StmtInfo, std::string>>& v) {
	if (e == EntityType::STMT) {
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

std::vector<std::vector<std::string>> Utility::filterResults(
	std::pair<EntityType, EntityType> type, std::vector<std::pair<StmtInfo, StmtInfo>>& table) {
	if (type.first == EntityType::STMT && type.second == EntityType::STMT) {
		return pairToStringTable(table);
	}

	std::vector < std::pair<StmtInfo, StmtInfo>> results;
	StmtType type1 = convertType(type.first);
	StmtType type2 = convertType(type.second);
	for (auto& row : table) {
		if ((type.first == EntityType::STMT || type1 == row.first.stmt_type) &&
			(type.second == EntityType::STMT || type2 == row.second.stmt_type)) {
			results.push_back(row);
		}
	}

	return pairToStringTable(results);
}

std::vector<std::vector<std::string>> Utility::filterResults(
	std::vector<std::vector<std::string>>& from, std::unordered_set<std::string>& filter, int index) {
	std::vector<std::vector<std::string>> results;

	for (auto& it : from) {
		if (filter.find(it[index]) != filter.end()) {
			results.push_back(it);
		}
	}

	return results;
}

std::vector<std::vector<std::string>> Utility::filterResults(std::vector<std::vector<std::string>>& from, 
	std::unordered_map<std::string, std::unordered_set<std::string>>& filter, int index1, int index2) {
	std::vector<std::vector<std::string>> results;

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

std::vector<std::string> Utility::mergeColumnEqual(std::vector<std::vector<std::string>>& v) {
	std::vector<std::string> to;
	for (unsigned int i = 0; i < v.size(); i++) {
		bool equal = true;
		for (unsigned int j = 1; j < v[i].size(); j++) {
			if (v[i][j - 1] != v[i][j]) {
				equal = false;
				break;
			}
		}
		if (equal) {
			to.push_back(v[i][0]);
		}
	}
	return to;
}

std::vector<std::vector<std::string>> Utility::joinTable(std::vector<std::vector<std::string>>& main, int main_header_index,
	std::unordered_multimap<std::string, std::vector<std::string>>& to_join, int to_join_header_index) {
	std::vector<std::vector<std::string>> results;

	for (auto& it : main) {
		if (to_join.count(it[main_header_index]) == 0) {
			continue;
		}
		std::pair<std::unordered_multimap<std::string, std::vector<std::string>>::iterator, std::unordered_multimap<std::string, std::vector<std::string>>::iterator> ret;

		ret = to_join.equal_range(it[main_header_index]);

		for (std::unordered_multimap<std::string, std::vector<std::string>>::iterator itr1 = ret.first; itr1 != ret.second; ++itr1) {
			std::vector<std::string> to(it);
			for (unsigned int i = 0; i < itr1->second.size(); i++) {
				if (i == to_join_header_index) {
					continue;
				}
				to.push_back(itr1->second[i]);
			}
			results.push_back(to);
		}
	}
	return results;
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
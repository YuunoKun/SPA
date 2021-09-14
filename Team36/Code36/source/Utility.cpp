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

std::vector<StmtInfo> Utility::stmtToStmtInfoVector(std::vector<Stmt>& from) {
	std::vector<StmtInfo> to;
	for (auto& it : from) {
		to.push_back({ it.getNum(), it.getType() });
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

std::vector<std::vector<std::string>> Utility::pairToStringTable(std::vector<std::pair<std::string, std::string>>& from) {
	std::vector< std::vector<std::string>> to;
	for (auto& it : from) {
		to.push_back({ it.first, it.second });
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
	case WHILE: return STMT_WHILE;
	case IF: return STMT_IF;
	case READ: return STMT_READ;
	case PRINT: return STMT_PRINT;
	case CALL: return STMT_CALL;
	case ASSIGN: return STMT_ASSIGN;
	}

	return StmtType();
}

std::vector<StmtInfo> Utility::filterResult(EntityType e, std::vector<StmtInfo>& v) {
	if (e == STMT) {
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

std::vector<std::vector<std::string>> Utility::filterResults(EntityType e, std::vector<std::pair<StmtInfo, std::string>>& v) {
	if (e == STMT) {
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

std::vector<std::vector<std::string>> Utility::filterResults(std::pair<EntityType, EntityType> type, std::vector<std::pair<StmtInfo, StmtInfo>>& table) {
	if (type.first == STMT && type.second == STMT) {
		return pairToStringTable(table);
	}

	std::vector < std::pair<StmtInfo, StmtInfo>> results;
	StmtType type1 = convertType(type.first);
	StmtType type2 = convertType(type.second);
	for (auto& row : table) {
		if ((type.first == STMT || type1 == row.first.stmt_type) &&
			(type.second == STMT || type2 == row.second.stmt_type)) {
			results.push_back(row);
		}
	}

	return pairToStringTable(results);
}

std::vector<std::vector<std::string>> Utility::filterResults(std::vector<std::vector<std::string>>& from, std::unordered_set<std::string>& filter, int index) {
	std::vector<std::vector<std::string>> results;

	for (auto& it : from) {
		if (filter.find(it[index]) != filter.end()) {
			results.push_back(it);
		}
	}

	return results;
}

std::vector<std::vector<std::string>> Utility::filterResults(std::vector<std::vector<std::string>>& from, std::unordered_map<std::string, std::unordered_set<std::string>>& filter, int index1, int index2) {
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

bool Utility::patternMatch(std::string a, std::string b) {
	a.erase(std::remove_if(a.begin(), a.end(), ::isspace), a.end());
	b.erase(std::remove_if(b.begin(), b.end(), ::isspace), b.end());
	return a == b;
}

bool Utility::patternContain(std::string original, std::string match) {
	original.erase(std::remove_if(original.begin(), original.end(), ::isspace), original.end());
	match.erase(std::remove_if(match.begin(), match.end(), ::isspace), match.end());

	const char delim = ' ';
	std::replace(original.begin(), original.end(), '*', delim); // replace all '*' to 'space'
	std::replace(original.begin(), original.end(), '/', delim); // replace all '/' to 'space'
	std::replace(original.begin(), original.end(), '+', delim); // replace all '+' to 'space'
	std::replace(original.begin(), original.end(), '-', delim); // replace all '-' to 'space'
	std::replace(original.begin(), original.end(), '%', delim); // replace all '-' to 'space'

	std::stringstream ss(original);
	std::vector<std::string> variables;

	std::string s;
	while (std::getline(ss, s, delim)) {
		if (s == match) {
			return true;
		}
	}
	return false;
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

std::vector<std::vector<std::string>> Utility::joinTable(std::vector<std::vector<std::string>>& main, int mainHeaderIndex,
	std::unordered_multimap<std::string, std::vector<std::string>>& toJoin, int toJoinHeaderIndex) {
	std::vector<std::vector<std::string>> results;

	for (auto& it : main) {
		if (toJoin.count(it[mainHeaderIndex]) == 0) {
			continue;
		}
		std::pair<std::unordered_multimap<std::string, std::vector<std::string>>::iterator, std::unordered_multimap<std::string, std::vector<std::string>>::iterator> ret;

		ret = toJoin.equal_range(it[mainHeaderIndex]);

		for (std::unordered_multimap<std::string, std::vector<std::string>>::iterator itr1 = ret.first; itr1 != ret.second; ++itr1) {
			std::vector<std::string> to(it);
			for (unsigned int i = 0; i < itr1->second.size(); i++) {
				if (i == toJoinHeaderIndex) {
					continue;
				}
				to.push_back(itr1->second[i]);
			}
			results.push_back(to);
		}
	}

	return results;
}

std::vector<std::vector<std::string>> Utility::joinTable(std::vector<std::vector<std::string>>& main, int mainIndex1, int mainIndex2,
	std::unordered_map<std::string, std::unordered_multimap<std::string, std::vector<std::string>>>& toJoin, int toJoinIndex1, int toJoinIndex2) {
	std::vector<std::vector<std::string>> results;

	for (auto& it : main) {
		auto it1 = toJoin.find(it[mainIndex1]);
		if (it1 == toJoin.end()) {
			continue;
		}

		std::pair<std::unordered_multimap<std::string, std::vector<std::string>>::iterator, std::unordered_multimap<std::string, std::vector<std::string>>::iterator> ret;

		ret = it1->second.equal_range(it[mainIndex2]);

		for (std::unordered_multimap<std::string, std::vector<std::string>>::iterator itr1 = ret.first; itr1 != ret.second; ++itr1) {
			std::vector<std::string> to(it);
			for (unsigned int i = 0; i < itr1->second.size(); i++) {
				if (i == toJoinIndex1 || i == toJoinIndex2) {
					continue;
				}
				to.push_back(itr1->second[i]);
			}
			results.push_back(to);
		}
	}

	return results;
}

EntityType Utility::queryTokenTypeToEntityType(QueryToken::QueryTokenType& queryTokenType) {
	if (queryTokenType == QueryToken::QueryTokenType::STMT) {
		return EntityType::STMT;
	}
	else if (queryTokenType == QueryToken::QueryTokenType::PROCEDURE) {
		return EntityType::PROCEDURE;
	}
	else if (queryTokenType == QueryToken::QueryTokenType::READ) {
		return EntityType::READ;
	}
	else if (queryTokenType == QueryToken::QueryTokenType::PRINT) {
		return EntityType::PRINT;
	}
	else if (queryTokenType == QueryToken::QueryTokenType::CALL) {
		return EntityType::CALL;
	}
	else if (queryTokenType == QueryToken::QueryTokenType::IF) {
		return EntityType::IF;
	}
	else if (queryTokenType == QueryToken::QueryTokenType::WHILE) {
		return EntityType::WHILE;
	}
	else if (queryTokenType == QueryToken::QueryTokenType::ASSIGN) {
		return EntityType::ASSIGN;
	}
	else if (queryTokenType == QueryToken::QueryTokenType::VARIABLE) {
		return EntityType::VARIABLE;
	}
	else if (queryTokenType == QueryToken::QueryTokenType::CONSTANT) {
		return EntityType::CONSTANT;
	}
}
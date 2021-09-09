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
		to.push_back({ std::to_string(it.first.stmt_index), std::to_string(it.second.stmt_index)});
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
		if((type.first == STMT || type1 == row.first.stmt_type) &&
			(type.second == STMT || type2 == row.second.stmt_type)){

			results.push_back(row);
		}
	}

	return pairToStringTable(results);
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


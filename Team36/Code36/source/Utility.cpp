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

std::list<std::string> Utility::stmtsToStringList(std::vector<Stmt>& from) {
	std::list<std::string> to;
	for (auto& it : from) {
		to.push_back(std::to_string(it.getNum()));
	}
	return to;
}

std::vector<std::string> Utility::stmtsToStringVector(std::vector<Stmt>& from) {
	std::vector<std::string> to;
	for (auto& it : from) {
		to.push_back(std::to_string(it.getNum()));
	}
	return to;
}

std::vector<std::vector<std::string>> Utility::stmtsTableToStringTable(std::vector<std::vector<Stmt>>& from) {
	std::vector< std::vector<std::string>> to;
	for (auto& it : from) {
		to.push_back(stmtsToStringVector(it));
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

std::vector<EntityType> Utility::entityToEntityTypeVector(std::vector<Entity>& entities) {
	std::vector<EntityType> type;
	for (auto& it : entities) {
		type.push_back(it.getType());
	}
	return type;
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

std::vector<Stmt> Utility::filterResult(EntityType e, std::vector<Stmt>& v) {
	if (e == STMT) {
		return v;
	}

	StmtType type = convertType(e);;
	std::vector<Stmt> result;
	for (auto& it : v) {
		if (it.getType() == type) {
			result.push_back(it);
		}
	}
	return result;
}

std::vector<std::vector<Stmt>> Utility::filterResults(std::vector<EntityType> et, std::vector<std::vector<Stmt>>& table) {
	std::vector<std::vector<Stmt>> results;

	if (isAllStmt(et)) {
		return table;
	}

	for (auto& row : table) {
		bool match = true;
		for (int i = 0; i < et.size(); i++) {
			if (et[i] == STMT) {
				continue;
			}
			StmtType type = convertType(et[i]);;
			if (row[i].getType() != type) {
				match = false;
				break;
			}
		}
		if (match) {
			results.push_back(row);
		}
	}

	return results;
}

bool Utility::isAllStmt(std::vector<EntityType> v) {
	for (auto& it : v) {
		if (it != STMT) {
			return false;
		}
	}
	return true;
}

#include "Utility.h"

std::list<std::string> Utility::constantsToStringList(std::vector<constant>& from) {
	std::list<std::string> to;
	for each (constant c in from) {
		to.push_back(std::to_string(c));
	}
	return to;
}

std::list<std::string> Utility::proceduresToStringList(std::vector<procedure_name>& from) {
	std::list<std::string> to;
	for each (procedure_name name in from) {
		to.push_back(name.c_str());
	}
	return to;
}

std::list<std::string> Utility::stmtsToStringList(std::vector<Stmt>& from) {
	std::list<std::string> to;
	for each (Stmt stmt in from) {
		to.push_back(std::to_string(stmt.getNum()));
	}
	return to;
}

std::list<std::string> Utility::variablesToStringList(std::vector<variable_name>& from) {
	std::list<std::string> to;
	for each (variable_name name in from) {
		to.push_back(name.c_str());
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
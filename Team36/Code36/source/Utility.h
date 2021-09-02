#pragma once

#include "Entity.h"
#include "Common.h"
#include "Stmt.h"

class Utility {
public:

	static std::list<std::string> constantsToStringList(std::vector<constant>&);
	static std::list<std::string> proceduresToStringList(std::vector<procedure_name>&);
	static std::list<std::string> stmtsToStringList(std::vector<Stmt>&);
	static std::list<std::string> variablesToStringList(std::vector<variable_name>&);
	static std::list<std::string> unorderedSetToStringList(std::unordered_set<std::string>&);

	static int getIndex(std::vector<std::string>, std::string);
	static int getIndex(std::vector<Entity>, Entity);
};

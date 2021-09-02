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

	/*
	template <class T>
	static int getIndex(std::vector<T>, T);
	*/
};



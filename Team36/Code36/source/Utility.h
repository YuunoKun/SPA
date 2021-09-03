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

	static std::vector<std::string> stmtsToStringVector(std::vector<Stmt>&);
	static  std::vector<std::vector<std::string>> stmtsTableToStringTable(std::vector<std::vector<Stmt>>&);


	static std::vector<EntityType> entityToEntityTypeVector(std::vector<Entity>&);

	static int getIndex(std::vector<std::string>, std::string);
	static int getIndex(std::vector<Entity>, Entity);

	static StmtType convertType(EntityType);
	static std::vector<Stmt> filterResult(EntityType, std::vector<Stmt>&);
	static std::vector<std::vector<Stmt>> filterResults(std::vector<EntityType>, std::vector<std::vector<Stmt>>&);

private:
	static bool isAllStmt(std::vector<EntityType>);
};

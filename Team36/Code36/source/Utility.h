#pragma once

#include "Entity.h"
#include "Common.h"
#include "Stmt.h"

class Utility {
public:

	static std::list<std::string> constantsToStringList(std::vector<constant>&);
	static std::list<std::string> proceduresToStringList(std::vector<proc_name>&);
	static std::list<std::string> stmtInfoToStringList(std::vector<StmtInfo>&);
	static std::list<std::string> variablesToStringList(std::vector<var_name>&);
	static std::list<std::string> unorderedSetToStringList(std::unordered_set<std::string>&);


	static std::vector<StmtInfo> stmtToStmtInfoVector(std::vector<Stmt>&);
	static std::vector<std::string> stmtInfoToStringVector(std::vector<StmtInfo>&);
	static  std::vector<std::vector<std::string>> pairToStringTable(std::vector<std::pair<std::string, std::string>>&);
	static  std::vector<std::vector<std::string>> pairToStringTable(std::vector<std::pair<StmtInfo, std::string>>&);
	static  std::vector<std::vector<std::string>> pairToStringTable(std::vector<std::pair<StmtInfo, StmtInfo>>&);


	static std::vector<EntityType> entityToEntityTypeVector(std::vector<Entity>&);

	static int getIndex(std::vector<std::string>, std::string);
	static int getIndex(std::vector<Entity>, Entity);

	static StmtType convertType(EntityType);
	static std::vector<StmtInfo> filterResult(EntityType, std::vector<StmtInfo>&);
	static std::vector<std::vector<std::string>> filterResults(EntityType, std::vector<std::pair<StmtInfo, std::string>>&);
	static std::vector<std::vector<std::string>> filterResults(std::pair<EntityType, EntityType>, std::vector<std::pair<StmtInfo, StmtInfo>>&);

};

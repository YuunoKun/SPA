#pragma once

#include <string>
#include <unordered_map>

#include "Entity.h"
#include "Common.h"
#include "QueryToken.h"

class Utility {
public:

	static std::list<std::string> constantsToStringList(std::vector<constant>&);
	static std::list<std::string> proceduresToStringList(std::vector<proc_name>&);
	static std::list<std::string> stmtInfoToStringList(std::vector<StmtInfo>&);
	static std::list<std::string> variablesToStringList(std::vector<var_name>&);
	static std::list<std::string> unorderedSetToStringList(std::unordered_set<std::string>&);

	static std::vector<std::string> stmtInfoToStringVector(std::vector<StmtInfo>&);
	static std::vector<std::string> stmtIndexToStringVector(std::vector<stmt_index>&);
	static std::vector<std::vector<std::string>> pairToStringTable(std::vector<std::pair<std::string, std::string>>&);
	static std::vector<std::vector<std::string>> pairToStringTable(std::vector<std::pair<stmt_index, std::string>>&);
	static std::vector<std::vector<std::string>> pairToStringTable(std::vector<std::pair<StmtInfo, std::string>>&);
	static std::vector<std::vector<std::string>> pairToStringTable(std::vector<std::pair<StmtInfo, StmtInfo>>&);

	static int getIndex(std::vector<std::string>, std::string);
	static int getIndex(std::vector<Entity>, Entity);

	static StmtType convertType(EntityType);
	static std::vector<StmtInfo> filterResult(EntityType, std::vector<StmtInfo>&);
	static std::vector<std::vector<std::string>> filterResults(EntityType, std::vector<std::pair<StmtInfo, std::string>>&);
	static std::vector<std::vector<std::string>> filterResults(std::pair<EntityType, EntityType>, std::vector<std::pair<StmtInfo, StmtInfo>>&);
	static std::vector<std::vector<std::string>> filterResults(std::vector<std::vector<std::string>>&, std::unordered_set<std::string>&, int);
	static std::vector<std::vector<std::string>> filterResults(std::vector<std::vector<std::string>>&, std::unordered_map<std::string, std::unordered_set<std::string>>&, int, int);

	static std::vector<std::string> mergeColumnEqual(std::vector<std::vector<std::string>>&);

	static std::vector<std::vector<std::string>> joinTable(std::vector<std::vector<std::string>>&, int, std::unordered_multimap<std::string, std::vector<std::string>>&, int);
	static std::vector<std::vector<std::string>> joinTable(std::vector<std::vector<std::string>>&, std::vector<std::vector<std::string>>&);

	static EntityType queryTokenTypeToEntityType(QueryToken::QueryTokenType&);
	static AttrRef queryTokenTypeToAttrRef(QueryToken::QueryTokenType&);

private:
	static bool isStmt(EntityType);
};

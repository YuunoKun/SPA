#pragma once

#include <string>
#include <unordered_map>

#include "Entity.h"
#include "Common.h"
#include "Query.h"
#include "QueryToken.h"
#include "SemanticErrorException.h"
#include "SyntacticErrorException.h"

class Utility {
public:

	static std::list<std::string> constantsToStringList(std::vector<constant>&);
	static std::list<std::string> proceduresToStringList(std::vector<proc_name>&);
	static std::list<std::string> stmtInfoToStringList(std::vector<StmtInfo>&);
	static std::list<std::string> variablesToStringList(std::vector<var_name>&);
	static std::list<std::string> unorderedSetToStringList(std::unordered_set<std::string>&);

	static std::list<std::string> stmtIndexToStringList(std::vector<stmt_index>&);

	static std::vector<std::pair<std::string, std::string>> stringListToStringPair(std::list<std::string>&);

	static std::list<std::vector<std::string>> pairToStringTable(std::vector<std::pair<std::string, std::string>>&);
	static std::list<std::vector<std::string>> pairToStringTable(std::vector<std::pair<stmt_index, std::string>>&);
	static std::list<std::vector<std::string>> pairToStringTable(std::vector<std::pair<StmtInfo, std::string>>&);
	static std::list<std::vector<std::string>> pairToStringTable(std::vector<std::pair<StmtInfo, StmtInfo>>&);

	static int getIndex(std::vector<std::string>, std::string);
	static int getIndex(std::vector<Entity>, Entity);

	static StmtType convertType(EntityType);
	static std::vector<StmtInfo> filterResult(EntityType, std::vector<StmtInfo>&);
	static std::list<std::vector<std::string>> filterResults(EntityType, std::vector<std::pair<StmtInfo, std::string>>&);
	static std::list<std::vector<std::string>> filterResults(std::pair<EntityType, EntityType>, std::vector<std::pair<StmtInfo, StmtInfo>>&);

	static std::list<std::vector<std::string>> filterResults(std::list<std::vector<std::string>>&, std::unordered_set<std::string>&, int);
	static std::list<std::vector<std::string>> filterResults(std::list<std::vector<std::string>>&, std::unordered_map<std::string, std::unordered_set<std::string>>&, int, int);

	static std::list<std::string> mergeColumnEqual(std::list<std::vector<std::string>>&);

	static void joinTable(std::list<std::vector<std::string>>&, int,
		std::unordered_multimap<std::string, std::vector<std::string>>&, int,
		std::list<std::vector<std::string>>&);
	static void joinTable(std::list<std::vector<std::string>>&, 
		std::list<std::vector<std::string>>&, std::list<std::vector<std::string>>&);

	static std::vector<Entity> removeEntities(std::vector<Entity>&, std::vector<Entity>&);
	static std::vector<Entity> removeDuplicateEntities(std::vector<Entity>&);

	static std::list<std::vector<std::string>> getColumnsNoDuplicate(std::list<std::vector<std::string>>&, std::vector<int>&);

	static bool isSecondaryAttribute(Entity);

	static EntityType queryTokenTypeToEntityType(QueryToken::QueryTokenType&);
	static AttrRef queryTokenTypeToAttrRef(QueryToken::QueryTokenType&);
	static std::string Utility::queryTokenTypeToExprString(std::vector<QueryToken>);
	
	static bool isStmtRef(Query&, std::vector<QueryToken>);
	static bool isStmtRef(Query&, std::vector<QueryToken>, EntityType);
	static bool isEntRef(Query&, std::vector<QueryToken>);
	static bool isLineRef(Query&, std::vector<QueryToken>);
	static bool isRef(Query&, std::vector<QueryToken>);
	static bool isExpr(std::vector<QueryToken>);
	static bool isCorrectSynEntRef(Query&, std::vector<QueryToken>, EntityType);
	static bool isWildCard(std::vector<QueryToken>);
	static bool isStringRefType(Query&, std::vector<QueryToken>);
	static bool isIntRefType(Query&, std::vector<QueryToken>);
	static Entity setStmtRef(Query&, QueryToken, EntityType);
	static Entity setEntRef(Query&, std::vector<QueryToken>, EntityType);
	static Entity setLineRef(Query&, QueryToken);
	static Entity setRef(Query&, std::vector<QueryToken>, EntityType, AttrRef);
	static std::string setExpr(std::vector<QueryToken>);
	static bool checkIsSemanticError(Query&);
	static std::vector<std::vector<QueryToken>> splitTokenChain(int, QueryToken::QueryTokenType, std::vector<QueryToken>);
private:
	static bool isStmt(EntityType);

	static void joinRow(std::vector<std::string>&, std::vector<std::string>&, std::vector<std::string>&);
	static void joinRow(std::vector<std::string>&, std::vector<std::string>&, int, std::vector<std::string>&);
};

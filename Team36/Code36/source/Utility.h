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

	static void constantToStringList(std::vector<constant>&, std::list<std::string>&);
	static void procedureToStringList(std::vector<proc_name>&, std::list<std::string>&);
	static void stmtInfoToStringList(std::vector<StmtInfo>&, std::list<std::string>&);
	static void variableToStringList(std::vector<var_name>&, std::list<std::string>&);
	static void unorderedSetToStringList(std::unordered_set<std::string>&, std::list<std::string>&);
	static void stmtIndexToStringList(std::vector<stmt_index>&, std::list<std::string>&);

	static void stringListToStringTablePair(std::list<std::string>&, std::list<std::vector<std::string>>&);


	static void constantToStringTable(std::vector<constant>&, std::list<std::vector<std::string>>&);
	static void stringToStringTable(std::vector<std::string>&, std::list<std::vector<std::string>>&);
	static void stringToStringTable(std::list<std::string>&, std::list<std::vector<std::string>>&);
	static void stmtInfoToStringTable(std::vector<StmtInfo>&, std::list<std::vector<std::string>>&);
	static void stmtIndexToStringTable(std::vector<stmt_index>&, std::list<std::vector<std::string>>&);

	static void pairToStringTable(std::vector<std::pair<StmtInfo, std::string>>&, std::list<std::vector<std::string>>&);
	static void pairToStringTable(std::vector<std::pair<StmtInfo, StmtInfo>>&, std::list<std::vector<std::string>>&);
	static void pairToStringTable(std::vector<std::pair<std::string, std::string>>&, std::list<std::vector<std::string>>&);
	static void pairToStringTable(std::vector<std::pair<stmt_index, std::string>>&, std::list<std::vector<std::string>>&);

	static int getIndex(std::vector<std::string>, std::string);
	static int getIndex(std::vector<Entity>, Entity);

	static StmtType convertType(EntityType);
	static std::vector<StmtInfo> filterResult(EntityType, std::vector<StmtInfo>&);
	static void filterResults(EntityType, std::vector<StmtInfo>&, std::list<std::vector<std::string>>&);
	static void filterResults(EntityType, std::vector<std::pair<StmtInfo, std::string>>&, std::list<std::vector<std::string>>&);
	static void filterResults(std::pair<EntityType, EntityType>, std::vector<std::pair<StmtInfo, StmtInfo>>&, std::list<std::vector<std::string>>&);

	static void filterResults(std::list<std::vector<std::string>>&, std::unordered_set<std::string>&, int, std::list<std::vector<std::string>>&);
	static void filterResults(std::list<std::vector<std::string>>&, std::unordered_map<std::string, std::unordered_set<std::string>>&, int, int, std::list<std::vector<std::string>>&);

	static void mergeColumnEqual(std::list<std::vector<std::string>>&);

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

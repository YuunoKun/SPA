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
	
	static void unorderedSetToStringList(std::unordered_set<std::string>&, std::list<std::string>&);


	static std::vector<std::pair<std::string, std::string>> duplicateColumn(std::vector<std::string>&);
	static std::vector<std::pair<value, value>> duplicateColumn(std::vector<value>&);
	static std::vector<std::pair<StmtInfo, StmtInfo>> duplicateColumn(std::vector<StmtInfo>&);

	static void stmtInfoToTable(std::vector<StmtInfo>&, std::list<std::vector<value>>&);
	static void valueToTable(std::vector<value>&, std::list<std::vector<value>>&);
	static void stringToTable(std::vector<std::string>&, std::list<std::vector<value>>&, std::unordered_map<value, std::string>&);
	static void stringToTable(std::list<std::string>&, std::list<std::vector<value>>&, std::unordered_map<value, std::string>&);

	static void pairToTable(std::vector<std::pair<StmtInfo, std::string>>&, std::list<std::vector<value>>&, std::unordered_map<value, std::string>&);
	static void pairToTable(std::vector<std::pair<StmtInfo, StmtInfo>>&, std::list<std::vector<value>>&);
	static void pairToTable(std::vector<std::pair<value, value>>&, std::list<std::vector<value>>&);
	static void pairToTable(std::vector<std::pair<std::string, std::string>>&, std::list<std::vector<value>>&, std::unordered_map<value, std::string>&);
	static void pairToTable(std::vector<std::pair<value, std::string>>&, std::list<std::vector<value>>&, std::unordered_map<value, std::string>&);

	static int getIndex(std::vector<Entity>, Entity);

	static std::vector<StmtInfo> filterResult(EntityType, std::vector<StmtInfo>&);
	static void filterResults(EntityType, std::vector<StmtInfo>&, std::list<std::vector<value>>&);
	static void filterResults(EntityType, std::vector<std::pair<StmtInfo, std::string>>&, std::list<std::vector<value>>&, std::unordered_map<value, std::string>&);
	static void filterResults(std::pair<EntityType, EntityType>, std::vector<std::pair<StmtInfo, StmtInfo>>&, std::list<std::vector<value>>&);

	static void filterResults(std::list<std::vector<value>>&, std::unordered_set<value>&, int, std::list<std::vector<value>>&);
	static void filterResults(std::list<std::vector<value>>&, std::unordered_map<value, std::unordered_set<value>>&, int, int, std::list<std::vector<value>>&);

	static void mergeColumnEqual(std::list<std::vector<value>>&);

	static void joinTable(std::list<std::vector<value>>&, int,
		std::unordered_multimap<value, std::vector<value>>&, int,
		std::list<std::vector<value>>&);
	static void joinTable(std::list<std::vector<value>>&,
		std::list<std::vector<value>>&, std::list<std::vector<value>>&);

	static std::vector<Entity> getEntitiesExclude(std::vector<Entity>&, std::vector<Entity>&);
	static std::vector<Entity> getEntitiesWithoutDuplicate(std::vector<Entity>&);

	static void getColumnsWithoutDuplicate(std::list<std::vector<value>>&, std::vector<int>&, std::list<std::vector<value>>&);

	static bool isSecondaryAttribute(Entity);

	static EntityType queryTokenTypeToEntityType(QueryToken::QueryTokenType&);
	static AttrRef queryTokenTypeToAttrRef(QueryToken::QueryTokenType&);
	static RelType queryTokenTypeToRelType(QueryToken::QueryTokenType&);
	static std::string Utility::queryTokenTypeToExprString(std::vector<QueryToken>);
	
	static void isSyntacticValidpattern(std::vector<QueryToken>);
	static bool isStmtRef(Query&, std::vector<QueryToken>);
	static bool isStmtRef(Query&, std::vector<QueryToken>, EntityType);
	static bool isEntRef(Query&, std::vector<QueryToken>, EntityType);
	static bool isSyntacticValidEntRef(std::vector<QueryToken>);
	static bool isSemanticValidEntRef(Query&, std::vector<QueryToken>, EntityType);
	static bool isLineRef(Query&, std::vector<QueryToken>);
	static bool isRef(Query&, std::vector<QueryToken>);
	static bool isExpr(std::vector<QueryToken>);
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

	static bool isStringEntityType(EntityType);
	static value hashString(std::string, std::unordered_map<value, std::string>&);

private:
	static StmtType convertType(EntityType);
	static bool isStmt(EntityType);

	static void joinRow(std::vector<value>&, std::vector<value>&, std::vector<value>&);
	static void joinRow(std::vector<value>&, std::vector<value>&, int, std::vector<value>&);

};

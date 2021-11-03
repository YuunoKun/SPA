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
	//static inline std::string const PLUS_S = "+";
	static constexpr char PLUS_C = '+';
	
	static std::string const EMPTY_STR;

	static std::string const SUCH_STR;
	static std::string const SUCH_SPACE_STR;
	static std::string const SUCH_SPACE_THAT_STR;
	static std::string const PROG_STR;
	static std::string const PROG_UNDERSCORE_STR;
	static std::string const PROG_UNDERSCORE_LINE_STR;

	static std::string const FOLLOWS_STR;
	static std::string const PARENT_STR;
	static std::string const CALLS_STR;
	static std::string const NEXT_STR;
	static std::string const NEXT_BIP_STR;
	static std::string const AFFECTS_STR;
	static std::string const AFFECTS_BIP_STR;

	static std::string const DOT_PROCNAME_STR;
	static std::string const DOT_VARNAME_STR;
	static std::string const DOT_VALIUE_STR;
	static std::string const DOT_STMT_STR;
	static std::string const DOT_STMT_HASH_STR;

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

	static std::list<std::vector<std::string>> joinTable(std::list<std::vector<std::string>>&, int, std::unordered_multimap<std::string, std::vector<std::string>>&, int);
	static std::list<std::vector<std::string>> joinTable(std::list<std::vector<std::string>>&, std::list<std::vector<std::string>>&);

	static std::vector<Entity> removeEntities(std::vector<Entity>&, std::vector<Entity>&);
	static std::vector<Entity> removeDuplicateEntities(std::vector<Entity>&);

	static std::list<std::vector<std::string>> getColumnsNoDuplicate(std::list<std::vector<std::string>>&, std::vector<int>&);

	static bool isSecondaryAttribute(Entity);

	static EntityType queryTokenTypeToEntityType(QueryToken::QueryTokenType&);
	static AttrRef queryTokenTypeToAttrRef(QueryToken::QueryTokenType&);
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
private:
	static bool isStmt(EntityType);

	static std::vector<std::string> joinRow(std::vector<std::string>&, std::vector<std::string>&);
	static std::vector<std::string> joinRow(std::vector<std::string>&, std::vector<std::string>&, int);
};

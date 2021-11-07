#pragma once

#include "PQL/QueryObject/Query.h"
#include "PQL/QueryEvaluator/QueryResult.h"
#include "PKB/PKBAdapter.h"
#include "PQL/Helper/Utility.h"
#include "ResultTable.h"

class QueryResult {
public:
	QueryResult();
	~QueryResult();

	bool haveResult();
	void setNoResult();
	bool isInTables(std::vector<Entity>);
	bool isInTables(Entity);
	void addResult(ResultTable&);
	void getResults(std::vector<Entity>&, ResultTable&);
	void getResult(Entity, std::list<std::string>&);

	void updateHaveResultAfterTableJoin();

private:
	ResultTable* generateNewResultTable(ResultTable&);

	bool tryMergeResultTableCommonHeader(ResultTable&, std::list<ResultTable*>&);
	bool tryMergeResultTableIntoCurrentTable(ResultTable&, std::list<ResultTable*>&);
	void mergeResultTableWithNewTable(ResultTable&, std::list<ResultTable*>&);

	bool mergeResultTable(ResultTable*, ResultTable&);
	void mergeResultTable(ResultTable&, std::list<ResultTable*>&);

	void QueryResult::updateAllAffectedTable(std::list<ResultTable*>& affected);

	bool joinResultTables(std::list<std::pair<Entity, Entity >>&);
	bool joinResultTables(std::list<std::pair<Entity, Entity >>&, std::vector<Entity> selected_list, ResultTable& out);
	bool joinResultTable(ResultTable&, std::pair<Entity, Entity>);
	bool joinResultTableExcludeJoinColumn(ResultTable&, std::pair<Entity, Entity>, Entity);

	void getSelectedEntitiesMergedTable(std::vector<Entity>, std::list<ResultTable>&);

	std::unordered_set<std::string> header_set;

	std::unordered_map<std::string, ResultTable*> results;

	bool have_result;
	void addHeader(std::vector<Entity>);
	std::list<std::pair<Entity, Entity>> getAllTableHeaderWithTwoSynonym();
};
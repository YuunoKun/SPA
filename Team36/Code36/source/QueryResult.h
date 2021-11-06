#pragma once

#include "Query.h"
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

private:

	ResultTable* generateNewResultTable(ResultTable&);


	bool tryMergeResultTableCommonHeader(ResultTable&, std::list<ResultTable*>&);
	bool tryMergeResultTableIntoCurrentTable(ResultTable&, std::list<ResultTable*>&);
	void mergeResultTableWithNewTable(ResultTable&, std::list<ResultTable*>&);

	bool mergeResultTable(ResultTable*, ResultTable&);
	void mergeResultTable(ResultTable&, std::list<ResultTable*>&);

	void QueryResult::updateAllAffectedTable(std::list<ResultTable*>& affected);

	void getMergedTable(std::vector<Entity>, std::list<ResultTable*>&);

	void getSelectedEntitiesMergedTable(std::vector<Entity>, std::list<ResultTable>&);
	
	std::unordered_set<std::string> header_set;

	std::unordered_map<std::string, ResultTable*> results;

	bool have_result;

	void addHeader(std::vector<Entity>);
};
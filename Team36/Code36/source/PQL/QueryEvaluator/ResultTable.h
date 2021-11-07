#pragma once

#include <unordered_map>
#include "PQL/QueryObject/Entity.h"
#include "PQL/Helper/Utility.h"

class ResultTable {
public:
	ResultTable();
	ResultTable(ResultTable&);
	ResultTable(Entity&, std::vector<StmtInfo>&);
	ResultTable(Entity&, std::vector<value>&);
	ResultTable(Entity&, std::vector<std::string>&);
	ResultTable(std::vector<Entity>&, std::list<std::vector<value>>&, std::unordered_map<value, std::string>&);
	ResultTable(std::pair<Entity, Entity>, std::vector<std::pair<std::string, std::string>>&);
	ResultTable(std::pair<Entity, Entity>, std::vector<std::pair<value, std::string>>&);
	ResultTable(std::pair<Entity, Entity>, std::vector<std::pair<value, value>>&);
	ResultTable(std::pair<Entity, Entity>, std::vector<std::pair<StmtInfo, StmtInfo>>&);
	ResultTable(std::pair<Entity, Entity>, std::vector<std::pair<StmtInfo, std::string>>&);

	void joinTable(ResultTable&);
	void tryJoinTableExcludeJoinColumn(ResultTable&, Entity);
	bool merge(ResultTable&);
	bool filter(ResultTable&);
	bool isInTable(Entity);
	bool isEmpty();
	size_t size();
	void getEntityResult(Entity&, std::list<std::string>&);
	void getEntityResults(std::vector<Entity>, std::list<std::list<std::string>>&);
	std::vector<Entity> getCommonHeaders(std::vector<Entity>&);
	ResultTable getResultTable(std::vector<Entity>&);
	std::vector<Entity> getHeaders();
	std::string getHeadersName();

	bool operator==(const ResultTable&) const;

private:
	void init(ResultTable&);

	void addHeader(Entity&);
	void addHeader(std::pair<Entity, Entity>&);
	void addHeader(std::vector<Entity>&);
	void removeHeader(Entity&);
	void removeColumn(Entity&);
	void addHashToStringMap(std::unordered_map<value, std::string>&);


	void generateHeaderName();

	bool header_outdated = false;
	std::string header_name;
	std::vector<Entity> header;
	std::unordered_set<std::string> header_set;
	std::list<std::vector<value>> table;
	std::unordered_map<value, std::string> hash_map;

	int getHeaderIndex(Entity);

	void filter_table(ResultTable&, Entity);
	void filter_table(ResultTable&, Entity, Entity);

	void joinTable(ResultTable&, Entity);
	void joinTableExcludeJoinColumn(ResultTable&, Entity);

	void getStringEntityResult(int, std::unordered_set<std::string>&);
	void getIntEntityResult(int, std::unordered_set<std::string>&);
};
#pragma once

#include "Entity.h"

class ResultTable {
public:
	ResultTable(Entity&, std::vector<StmtInfo>&);
	ResultTable(Entity&, std::vector<stmt_index>&);
	ResultTable(Entity&, std::vector<std::string>&);
	ResultTable(Entity&, std::list<std::string>&);
	ResultTable(std::pair<Entity, Entity>, std::vector<std::pair<std::string, std::string>>&);
	ResultTable(std::pair<Entity, Entity>, std::vector<std::pair<stmt_index, std::string>>&);
	ResultTable(std::pair<Entity, Entity>, std::vector<std::pair<StmtInfo, StmtInfo>>&);
	ResultTable(std::pair<Entity, Entity>, std::vector<std::pair<StmtInfo, std::string>>&);
	ResultTable(std::vector<Entity>&, std::list<std::vector<std::string>>&);

	void joinTable(ResultTable&);
	bool merge(ResultTable&);
	bool isInTable(Entity);
	bool isEmpty();
	std::list<std::string> getEntityResult(Entity);
	std::list<std::list<std::string>> getEntityResults(std::vector<Entity>);
	std::vector<Entity> getCommonHeaders(std::vector<Entity>&);
	ResultTable getResultTable(std::vector<Entity>&);
	std::vector<Entity> getHeaders();

	bool operator==(const ResultTable&) const;

private:
	void init(Entity, std::list<std::string>&);
	void init(std::pair<Entity, Entity>, std::list<std::vector<std::string>>&);

	void addHeader(Entity);
	void addHeader(std::vector<Entity>&);

	std::vector<Entity> header;
	std::unordered_set<std::string> header_set;
	std::list<std::vector<std::string>> table;

	int getHeaderIndex(Entity);

	void filter_table(ResultTable&, Entity);
	void filter_table(ResultTable&, Entity, Entity);

	void joinTable(ResultTable&, Entity);
};
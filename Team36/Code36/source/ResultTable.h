#pragma once

#include "Entity.h"
#include "Stmt.h"

class ResultTable {
public:
	ResultTable(Entity&, std::vector<StmtInfo>&);
	ResultTable(Entity&, std::vector<stmt_index>&);
	ResultTable(Entity&, std::vector<std::string>&);
	ResultTable(std::pair<Entity, Entity>, std::vector<std::pair<std::string, std::string>>&);
	ResultTable(std::pair<Entity, Entity>, std::vector<std::pair<stmt_index, std::string>>&);
	ResultTable(std::pair<Entity, Entity>, std::vector<std::pair<StmtInfo, StmtInfo>>&);
	ResultTable(std::pair<Entity, Entity>, std::vector<std::pair<StmtInfo, std::string>>&);

	bool merge(ResultTable);
	bool isInTable(Entity);
	bool isEmpty();
	std::list<std::string> getEntityResult(Entity);
	std::vector<Entity> getCommonHeaders(std::vector<Entity>);
	std::vector<Entity> getHeaders();

	bool operator==(const ResultTable&) const;

private:
	void init(Entity, std::vector<std::string>);
	void init(std::pair<Entity, Entity>, std::vector<std::vector<std::string>>);

	void addHeader(Entity);
	void addHeader(std::vector<Entity>);

	std::vector<Entity> header;
	std::unordered_set<std::string> header_set;
	std::vector<std::vector<std::string>> table;

	int getHeaderIndex(Entity);

	void filter_table(ResultTable, Entity);
	void filter_table(ResultTable, Entity, Entity);

	void joinTable(ResultTable, Entity);
};
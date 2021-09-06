#pragma once

#include "Entity.h"
#include "Stmt.h"

class ResultTable {
public:
	ResultTable(Entity&, std::vector<StmtInfo>&);
	ResultTable(Entity&, std::vector<std::string>&);
	ResultTable(std::vector<Entity>&, std::vector<std::pair<std::string, std::string>>&);
	ResultTable(std::vector<Entity>&, std::vector<std::pair<StmtInfo, StmtInfo>>&);
	ResultTable(std::vector<Entity>&, std::vector<std::pair<StmtInfo, std::string>>&);

	bool merge(ResultTable);
	bool isInTable(Entity);
	bool isEmpty();
	std::list<std::string> getEntityResult(Entity);

private:
	void init(Entity, std::vector<std::string>);
	void init(std::vector<Entity>, std::vector<std::vector<std::string>>);

	std::vector<Entity> header;
	std::vector<std::vector<std::string>> table;
};
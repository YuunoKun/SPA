#pragma once

#include "Entity.h"
#include "Stmt.h"

class ResultTable {
public:
	ResultTable(Entity&, std::vector<Stmt>&);
	ResultTable(Entity&, std::vector<std::string>&);
	ResultTable(std::vector<Entity>&, std::vector<std::vector<Stmt>>&);
	ResultTable(std::vector<Entity>&, std::vector<std::vector<std::string>>&);

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
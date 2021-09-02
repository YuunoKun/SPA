#pragma once

#include "Entity.h"


class ResultTable {
public:
	ResultTable(std::vector<Entity>, std::vector<std::vector<std::string>>);
	
	bool merge(ResultTable);
	bool isInTable(Entity);
	std::vector<std::string> getEntityResult(Entity);

private:
	std::vector<Entity> header;
	std::vector<std::vector<std::string>> table;
};
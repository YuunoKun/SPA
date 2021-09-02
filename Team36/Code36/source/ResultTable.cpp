#include "ResultTable.h"

ResultTable::ResultTable(std::vector<Entity> header, std::vector<std::vector<std::string>> table) {
	this->header = header;
	this->table = table;
}

//TODO
bool ResultTable::merge(ResultTable) {
	return false;
}

bool ResultTable::isInTable(Entity e) {
	return std::find(header.begin(), header.end(), e) != header.end();
}

std::vector<std::string> ResultTable::getEntityResult(Entity) {
	header.
	return std::vector<std::string>();
}

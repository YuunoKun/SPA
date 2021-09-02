#include "ResultTable.h"
#include "Utility.h"

ResultTable::ResultTable(std::vector<Entity> header, std::vector<std::vector<std::string>> table) {
	this->header = header;
	this->table = table;
}

//Return true if merge result in an non-empty table
bool ResultTable::merge(ResultTable t) {
	//TODO
	return false;
}

bool ResultTable::isInTable(Entity e) {
	return std::find(header.begin(), header.end(), e) != header.end();
}

std::list<std::string> ResultTable::getEntityResult(Entity e) {
	int columnIndex = Utility::getIndex(header, e);
	std::unordered_set<std::string> result;
	for (int i = 0; i < table.size(); i++) {
		result.insert(table[i][columnIndex]);
	}
	return Utility::unorderedSetToStringList(result);
}
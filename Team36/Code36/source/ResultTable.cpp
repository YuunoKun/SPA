#include "ResultTable.h"
#include "Utility.h"


ResultTable::ResultTable(Entity& header, std::vector<StmtInfo>& table) {
	table = Utility::filterResult(header.getType(), table);
	init(header, Utility::stmtInfoToStringVector(table));
}

ResultTable::ResultTable(Entity& header, std::vector<std::string>& table) {
	init(header, table);
}

ResultTable::ResultTable(std::vector<Entity>& header, std::vector<std::pair<std::string, std::string>>& table) {
	init(header, Utility::pairToStringTable(table));
}

ResultTable::ResultTable(std::vector<Entity>& header, std::vector<std::pair<StmtInfo, std::string>>& table) {
	std::vector<EntityType> types = Utility::entityToEntityTypeVector(header);
	init(header, Utility::filterResults(types[0], table));
}

ResultTable::ResultTable(std::vector<Entity>& header, std::vector<std::pair<StmtInfo, StmtInfo>>& table) {
	std::vector<EntityType> types = Utility::entityToEntityTypeVector(header);
	init(header, Utility::filterResults(std::make_pair(types[0], types[1]), table));
}

//Return true if merge is successful
bool ResultTable::merge(ResultTable t) {
	//TODO

	return false;
}

bool ResultTable::isInTable(Entity e) {
	return std::find(header.begin(), header.end(), e) != header.end();
}

//if table contain no value
bool ResultTable::isEmpty() {
	return table.size() == 0;
}

std::list<std::string> ResultTable::getEntityResult(Entity e) {
	int columnIndex = Utility::getIndex(header, e);
	std::unordered_set<std::string> result;
	for (unsigned int i = 0; i < table.size(); i++) {
		result.insert(table[i][columnIndex]);
	}
	return Utility::unorderedSetToStringList(result);
}

void ResultTable::init(Entity header, std::vector<std::string> table) {
	this->header = { header };
	for (auto& it : table) {
		this->table.push_back({ it });
	}
}

void ResultTable::init(std::vector<Entity> header, std::vector<std::vector<std::string>> table) {
	this->header = header;
	this->table = table;
}

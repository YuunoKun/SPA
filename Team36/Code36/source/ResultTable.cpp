#include "ResultTable.h"
#include "Utility.h"

ResultTable::ResultTable(Entity& header, std::vector<StmtInfo>& table) {
	table = Utility::filterResult(header.getType(), table);
	init(header, Utility::stmtInfoToStringVector(table));
}

ResultTable::ResultTable(Entity& header, std::vector<std::string>& table) {
	init(header, table);
}

ResultTable::ResultTable(std::pair<Entity, Entity> header, std::vector<std::pair<std::string, std::string>>& table) {
	init(header, Utility::pairToStringTable(table));
}

ResultTable::ResultTable(std::pair<Entity, Entity>  header, std::vector<std::pair<StmtInfo, std::string>>& table) {
	init(header, Utility::filterResults(header.first.getType(), table));
}

ResultTable::ResultTable(std::pair<Entity, Entity>  header, std::vector<std::pair<StmtInfo, StmtInfo>>& table) {
	init(header, Utility::filterResults(std::make_pair(header.first.getType(), header.second.getType()), table));
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

bool ResultTable::operator==(const ResultTable& other) const {
	return header == other.header && table == other.table;
}

void ResultTable::init(Entity header, std::vector<std::string> table) {
	this->header = { header };
	for (auto& it : table) {
		this->table.push_back({ it });
	}
}

void ResultTable::init(std::pair<Entity, Entity> header, std::vector<std::vector<std::string>> table) {
	this->header = { header.first, header.second };
	this->table = table;
}
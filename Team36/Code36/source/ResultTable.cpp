#include "ResultTable.h"
#include "Utility.h"


ResultTable::ResultTable(Entity& header, std::vector<Stmt>& table) {
	table = Utility::filterResult(header.getType(), table);
	init(header, Utility::stmtsToStringVector(table));
}

ResultTable::ResultTable(Entity& header, std::vector<std::string>& table) {
	init(header, table);
}

ResultTable::ResultTable(std::vector<Entity>& header, std::vector<std::vector<Stmt>>& table) {
	std::vector<EntityType> types = Utility::entityToEntityTypeVector(header);
	table = Utility::filterResults(types, table);
	init(header, Utility::stmtsTableToStringTable(table));
}

ResultTable::ResultTable(std::vector<Entity>& header, std::vector<std::vector<std::string>>& table) {
	init(header, table);
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
	for (int i = 0; i < table.size(); i++) {
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

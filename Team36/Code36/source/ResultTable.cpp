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
	std::vector<Entity> commonHeaders = getCommonHeaders(t.header);
	//No common header, abandon merge.
	if (commonHeaders.empty()) {
		return false;
	}

	if (commonHeaders.size() == 1 && (t.header.size() == 1 || header.size() == 1)) {
		//If table a or b only have 1 column, filter the result
		filterTable(t, commonHeaders[0]);
	}
	else if (commonHeaders.size() == 2 && (t.header.size() == 2 || header.size() == 2)) {
		//If table a or b only have 2 column, and both synonym is in the commonHeader, filter the result
		filterTable(t, commonHeaders[0], commonHeaders[1]);
	}
	else if (commonHeaders.size() == 1) {
		//If table a and b have more than 1 column and there is 1 common header, join table
		joinTable(t, commonHeaders[0]);
	}
	else {
		throw std::exception("Error: table merging scenario is not handled!!!");
	}

	return true;
}

bool ResultTable::isInTable(Entity e) {
	return headerSet.find(e.getSynonym()) != headerSet.end();
}

//if table contain no value
bool ResultTable::isEmpty() {
	return table.empty();
}

std::list<std::string> ResultTable::getEntityResult(Entity e) {
	int columnIndex = Utility::getIndex(header, e);
	std::unordered_set<std::string> result;
	for (unsigned int i = 0; i < table.size(); i++) {
		result.insert(table[i][columnIndex]);
	}
	return Utility::unorderedSetToStringList(result);
}

std::vector<Entity> ResultTable::getCommonHeaders(std::vector<Entity> v) {
	std::vector<Entity> commonHeaders;

	for (unsigned int i = 0; i < v.size(); i++) {
		if (headerSet.find(v[i].getSynonym()) != headerSet.end()) {
			commonHeaders.push_back(v[i]);
		}
	}

	return commonHeaders;
}

std::vector<Entity> ResultTable::getHeaders() {
	return header;
}

bool ResultTable::operator==(const ResultTable& other) const {
	return header == other.header && table == other.table;
}

void ResultTable::init(Entity header, std::vector<std::string> table) {
	addHeader(header);
	for (auto& it : table) {
		this->table.push_back({ it });
	}
}

void ResultTable::init(std::pair<Entity, Entity> header, std::vector<std::vector<std::string>> table) {
	if (header.first == header.second) {
		//If the header is the same, merge column if equal.
		init(header.first, Utility::mergeColumnEqual(table));
	}
	else {
		addHeader(header.first);
		addHeader(header.second);
		this->table = table;
	}
}

void ResultTable::addHeader(Entity entity) {
	if (headerSet.count(entity.getSynonym()) == 0) {
		header.push_back(entity);
		headerSet.insert(entity.getSynonym());
	}
}

void ResultTable::addHeader(std::vector<Entity> v) {
	for (auto& it : v) {
		addHeader(it);
	}
}

int ResultTable::getHeaderIndex(Entity e) {
	for (unsigned int i = 0; i < header.size(); i++) {
		if (e == header[i]) {
			return i;
		}
	}
	return -1;
}

void ResultTable::filterTable(ResultTable t, Entity commonHeader) {
	int headerIndex;
	std::vector<std::vector<std::string>> mainTable;
	std::vector<std::vector<std::string>> filterTable;

	if (t.header.size() == 1) {
		headerIndex = getHeaderIndex(commonHeader);
		mainTable = table;
		filterTable = t.table;
	}
	else if (header.size() == 1) {
		headerIndex = t.getHeaderIndex(commonHeader);
		mainTable = t.table;
		filterTable = table;
		header = t.header;
		headerSet = t.headerSet;
	}
	else {
		throw std::exception("Error: table filter 1 column scenario is not handled!!!");
	}

	std::unordered_set<std::string> filter;
	for (auto& it : filterTable) {
		filter.insert(it[0]);
	}

	table = Utility::filterResults(mainTable, filter, headerIndex);
}

void ResultTable::filterTable(ResultTable t, Entity commonHeader1, Entity commonHeader2) {
	int headerIndex1;
	int headerIndex2;
	int filterIndex1;
	int filterIndex2;
	std::vector<std::vector<std::string>> mainTable;
	std::vector<std::vector<std::string>> filterTable;

	if (t.header.size() == 2) {
		headerIndex1 = getHeaderIndex(commonHeader1);
		headerIndex2 = getHeaderIndex(commonHeader2);
		filterIndex1 = t.getHeaderIndex(commonHeader1);
		filterIndex2 = t.getHeaderIndex(commonHeader2);
		mainTable = table;
		filterTable = t.table;
	}
	else if (header.size() == 2) {
		headerIndex1 = t.getHeaderIndex(commonHeader1);
		headerIndex2 = t.getHeaderIndex(commonHeader2);
		filterIndex1 = getHeaderIndex(commonHeader1);
		filterIndex2 = getHeaderIndex(commonHeader2);
		mainTable = t.table;
		filterTable = table;
		header = t.header;
		headerSet = t.headerSet;
	}
	else {
		throw std::exception("Error: table filter 2 column scenario is not handled!!!");
	}

	std::unordered_map<std::string, std::unordered_set<std::string>> filters;
	for (auto& it : filterTable) {
		if (filters.find(it[filterIndex1]) == filters.end()) {
			filters.insert({ it[filterIndex1], {} });
		}
		auto container = filters.find(it[filterIndex1]);
		container->second.insert(it[filterIndex2]);
	}

	table = Utility::filterResults(mainTable, filters, headerIndex1, headerIndex2);
}

void ResultTable::joinTable(ResultTable t, Entity commonHeader) {
	int headerIndex = getHeaderIndex(commonHeader);
	int toJoinIndex = t.getHeaderIndex(commonHeader);

	std::unordered_multimap<std::string, std::vector<std::string>> toJoin;

	for (auto& it : t.table) {
		toJoin.insert({ it[toJoinIndex], it });
	}

	table = Utility::joinTable(table, headerIndex, toJoin, toJoinIndex);
	addHeader(t.header);
}

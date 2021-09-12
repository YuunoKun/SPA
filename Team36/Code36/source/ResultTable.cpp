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

	if (commonHeaders.empty()) {
		return false;
	}

	//If table to merge only have 1 column, filter the result
	if (t.header.size() == 1 && commonHeaders.size() == 1) {
		std::unordered_set<std::string> filter;
		for (auto& it : t.table) {
			filter.insert(it[0]);
		}
		int headerIndex = getHeaderIndex(commonHeaders[0]);
		table = Utility::filterResults(table, filter, headerIndex);
	}
	else if (t.header.size() == 2 && commonHeaders.size() == 2) {
		//If table have two column, and both synonym is in table, filter result
		int headerIndex1 = getHeaderIndex(commonHeaders[0]);
		int headerIndex2 = getHeaderIndex(commonHeaders[1]);

		std::unordered_map<std::string, std::unordered_set<std::string>> filters;
		for (auto& it : t.table) {
			if (filters.find(it[0]) == filters.end()) {
				filters.insert({ it[0], {} });
			}
			auto container = filters.find(it[0]);
			container->second.insert(it[1]);
		}

		table = Utility::filterResults(table, filters, headerIndex1, headerIndex2);
	}
	else if (commonHeaders.size() == 1 && t.header.size() > 1) {
		//If table have 1 common synonym, and more than 1 column, Join Table
		int headerIndex = getHeaderIndex(commonHeaders[0]);
		int toJoinIndex = t.getHeaderIndex(commonHeaders[0]);

		std::unordered_multimap<std::string, std::vector<std::string>> toJoin;

		for (auto& it : t.table) {
			toJoin.insert({ it[toJoinIndex], it });
		}

		table = Utility::joinTable(table, headerIndex, toJoin, toJoinIndex);
	}
	else if (commonHeaders.size() == 2 && t.header.size() > 2) {
		//If table have 2 common synonym, and more than 2 column, Join Table
		int headerIndex1 = getHeaderIndex(commonHeaders[0]);
		int headerIndex2 = getHeaderIndex(commonHeaders[1]);
		int toJoinIndex1 = t.getHeaderIndex(commonHeaders[0]);
		int toJoinIndex2 = t.getHeaderIndex(commonHeaders[1]);

		std::unordered_map<std::string, std::unordered_multimap<std::string, std::vector<std::string>>> toJoin;

		for (auto& it : t.table) {

			if (toJoin.find(it[toJoinIndex1]) == toJoin.end()) {
				toJoin.insert({ it[toJoinIndex1], {} });
			}
			auto container = toJoin.find(it[toJoinIndex1]);
			container->second.insert({ it[toJoinIndex2], it });
		}

		table = Utility::joinTable(table, headerIndex1, headerIndex2, toJoin, toJoinIndex1, toJoinIndex2);
	}
	else {
		throw std::exception("Error: table merging scenario is not handled!!!");
	}

	addHeader(t.header);

	return true;
}

bool ResultTable::isInTable(Entity e) {
	return headerSet.count(e.getSynonym()) > 0;
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
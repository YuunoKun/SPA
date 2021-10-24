#include "ResultTable.h"
#include "Utility.h"

ResultTable::ResultTable(Entity& header, std::vector<StmtInfo>& table) {
	table = Utility::filterResult(header.getType(), table);
	init(header, Utility::stmtInfoToStringList(table));
}

ResultTable::ResultTable(Entity& header, std::vector<stmt_index>& table) {
	init(header, Utility::stmtIndexToStringList(table));
}

ResultTable::ResultTable(Entity& header, std::vector<std::string>& table) {
	init(header, std::list<std::string>(table.begin(), table.end()));
}

ResultTable::ResultTable(std::pair<Entity, Entity> header, std::vector<std::pair<std::string, std::string>>& table) {
	init(header, Utility::pairToStringTable(table));
}

ResultTable::ResultTable(std::pair<Entity, Entity> header, std::vector<std::pair<stmt_index, std::string>>& table) {
	init(header, Utility::pairToStringTable(table));
}

ResultTable::ResultTable(std::pair<Entity, Entity>  header, std::vector<std::pair<StmtInfo, std::string>>& table) {
	init(header, Utility::filterResults(header.first.getType(), table));
}

ResultTable::ResultTable(std::pair<Entity, Entity>  header, std::vector<std::pair<StmtInfo, StmtInfo>>& table) {
	init(header, Utility::filterResults(std::make_pair(header.first.getType(), header.second.getType()), table));
}

//Return true if merge is successful
bool ResultTable::merge(ResultTable& t) {
	std::vector<Entity> common_headers = getCommonHeaders(t.header);
	//No common header, abandon merge.
	if (common_headers.empty()) {
		return false;
	}

	if (common_headers.size() == 1 && (t.header.size() == 1 || header.size() == 1)) {
		//If table a or b only have 1 column, filter the result
		filter_table(t, common_headers[0]);
	}
	else if (common_headers.size() == 2 && (t.header.size() == 2 || header.size() == 2)) {
		//If table a or b only have 2 column, and both synonym is in the common_header, filter the result
		filter_table(t, common_headers[0], common_headers[1]);
	}
	else if (common_headers.size() == 1) {
		//If table a and b have more than 1 column and there is 1 common header, join table
		joinTable(t, common_headers[0]);
	}
	else {
		throw std::exception("Error: table merging scenario is not handled!!!");
	}

	return true;
}

bool ResultTable::isInTable(Entity e) {
	return header_set.find(e.getSynonym()) != header_set.end();
}

//if table contain no value
bool ResultTable::isEmpty() {
	return table.empty();
}

std::list<std::string> ResultTable::getEntityResult(Entity e) {
	int columnIndex = Utility::getIndex(header, e);
	std::unordered_set<std::string> result;
	for (auto row : table) {
		result.insert(row[columnIndex]);
	}
	return Utility::unorderedSetToStringList(result);
}

std::vector<Entity> ResultTable::getCommonHeaders(std::vector<Entity>& v) {
	std::vector<Entity> common_headers;

	for (unsigned int i = 0; i < v.size(); i++) {
		if (header_set.find(v[i].getSynonym()) != header_set.end()) {
			common_headers.push_back(v[i]);
		}
	}

	return common_headers;
}

std::vector<Entity> ResultTable::getHeaders() {
	return header;
}

bool ResultTable::operator==(const ResultTable& other) const {
	return header == other.header && table == other.table;
}

void ResultTable::init(Entity header, std::list<std::string>& table) {
	addHeader(header);
	for (auto& it : table) {
		this->table.push_back({ it });
	}
}

void ResultTable::init(std::pair<Entity, Entity> header, std::list<std::vector<std::string>>& table) {
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
	if (header_set.count(entity.getSynonym()) == 0) {
		header.push_back(entity);
		header_set.insert(entity.getSynonym());
	}
}

void ResultTable::addHeader(std::vector<Entity>& v) {
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

void ResultTable::filter_table(ResultTable& t, Entity common_header) {
	int header_index;
	std::list<std::vector<std::string>> main_table;
	std::list<std::vector<std::string>> filter_table;

	if (t.header.size() == 1) {
		header_index = getHeaderIndex(common_header);
		main_table = table;
		filter_table = t.table;
	}
	else if (header.size() == 1) {
		header_index = t.getHeaderIndex(common_header);
		main_table = t.table;
		filter_table = table;
		header = t.header;
		header_set = t.header_set;
	}
	else {
		throw std::exception("Error: table filter 1 column scenario is not handled!!!");
	}

	std::unordered_set<std::string> filter;
	for (auto& it : filter_table) {
		filter.insert(it[0]);
	}

	table = Utility::filterResults(main_table, filter, header_index);
}

void ResultTable::filter_table(ResultTable& t, Entity common_header1, Entity common_header2) {
	int header_index1;
	int header_index2;
	int filter_index1;
	int filter_index2;
	std::list<std::vector<std::string>> main_table;
	std::list<std::vector<std::string>> filter_table;

	if (t.header.size() == 2) {
		header_index1 = getHeaderIndex(common_header1);
		header_index2 = getHeaderIndex(common_header2);
		filter_index1 = t.getHeaderIndex(common_header1);
		filter_index2 = t.getHeaderIndex(common_header2);
		main_table = table;
		filter_table = t.table;
	}
	else if (header.size() == 2) {
		header_index1 = t.getHeaderIndex(common_header1);
		header_index2 = t.getHeaderIndex(common_header2);
		filter_index1 = getHeaderIndex(common_header1);
		filter_index2 = getHeaderIndex(common_header2);
		main_table = t.table;
		filter_table = table;
		header = t.header;
		header_set = t.header_set;
	}
	else {
		throw std::exception("Error: table filter 2 column scenario is not handled!!!");
	}

	std::unordered_map<std::string, std::unordered_set<std::string>> filters;
	for (auto& it : filter_table) {
		if (filters.find(it[filter_index1]) == filters.end()) {
			filters.insert({ it[filter_index1], {} });
		}
		auto container = filters.find(it[filter_index1]);
		container->second.insert(it[filter_index2]);
	}

	table = Utility::filterResults(main_table, filters, header_index1, header_index2);
}

void ResultTable::joinTable(ResultTable& t, Entity common_header) {
	int header_index = getHeaderIndex(common_header);
	int to_join_index = t.getHeaderIndex(common_header);

	std::unordered_multimap<std::string, std::vector<std::string>> to_join;

	for (auto& it : t.table) {
		to_join.insert({ it[to_join_index], it });
	}

	table = Utility::joinTable(table, header_index, to_join, to_join_index);
	addHeader(t.header);
}



void ResultTable::joinTable(ResultTable& t) {
	table = Utility::joinTable(table, t.table);
	addHeader(t.header);
}
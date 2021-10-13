#include "QueryResult.h"
#include <stdexcept>

QueryResult::QueryResult() {
	have_result = true;
}

bool QueryResult::haveResult() {
	return have_result;
}

void QueryResult::setNoResult() {
	have_result = false;
}

bool QueryResult::isInTables(std::vector<Entity> v) {
	//for advance requirement
	for (auto& it : v) {
		if (isInTables(it)) {
			return true;
		}
	}
	return false;
}

bool QueryResult::isInTables(Entity e) {
	if (header_set.find(e.getSynonym()) != header_set.end()) {
		return true;
	}
	return false;
}

void QueryResult::addResult(ResultTable t) {
	if (t.isEmpty()) {
		have_result = false;
		return;
	}
	//If new result table cannot be merge
	if (!isInTables(t.getHeaders())) {
		results.push_back(t);
		addHeader(t.getHeaders());
		return;
	}
	addHeader(t.getHeaders());

	//Common header found, began merge
	std::vector<ResultTable> new_results;
	while (!results.empty()) {
		ResultTable r = results.back();
		results.pop_back();
		bool merged = t.merge(r);
		if (merged && t.isEmpty()) {
			have_result = false;
			return;
		}else if(!merged) {
			new_results.push_back(r);
		}
	}

	new_results.push_back(t);
	results = new_results;
}

void QueryResult::addHeader(std::vector<Entity> v) {
	for (auto& it : v) {
		header_set.insert(it.getSynonym());
	}
}

std::list<std::string> QueryResult::getResults(std::vector<Entity> e) {
	return getResult(e[0]);
}

std::list<std::string> QueryResult::getResult(Entity e) {
	for (auto& table : results) {
		if (table.isInTable(e)) {
			return table.getEntityResult(e);
		}
	}
	throw std::domain_error("Invalid Entity, Source: QueryResult.getResult");
}
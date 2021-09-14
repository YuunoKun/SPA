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
	if (headerSet.find(e.getSynonym()) != headerSet.end()) {
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
	std::vector<ResultTable> newResults;
	while (!results.empty()) {
		ResultTable r = results.back();
		results.pop_back();
		if (t.merge(r)) {
			if (t.isEmpty()) {
				have_result = false;
				return;
			}
		}
		else {
			newResults.push_back(r);
		}
	}

	newResults.push_back(t);
	results = newResults;
}

void QueryResult::addHeader(std::vector<Entity> v) {
	for (auto& it : v) {
		headerSet.insert(it.getSynonym());
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
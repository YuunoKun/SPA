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

bool QueryResult::isInTables(std::vector<Entity> e) {
	//for advance requirement
	return isInTables(e[0]);
}

bool QueryResult::isInTables(Entity e) {
	for (auto& table : results) {
		if (table.isInTable(e)) {
			return true;
		}
	}
	return false;
}

void QueryResult::addResult(ResultTable t) {
	for (auto& table : results) {
		if (table.merge(t)) {
			if (table.isEmpty()) {
				have_result = false;
			}
			return;
		}
	}
	results.push_back(t);
	if (t.isEmpty()) {
		have_result = false;
	}
}

std::list<std::string> QueryResult::getResult(Entity e) {
	for (auto& table : results) {
		if (table.isInTable(e)) {
			return table.getEntityResult(e);
		}
	}
	throw std::domain_error("Invalid Entity, Source: QueryResult.getResult");
}

std::list<std::string> QueryResult::getResult(std::vector<Entity> e) {
	return getResult(e[0]);
	throw std::domain_error("Invalid Entity, Source: QueryResult.getResult");
}
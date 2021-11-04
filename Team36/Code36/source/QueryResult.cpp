#include "QueryResult.h"
#include <stdexcept>
#include "Utility.h"

QueryResult::QueryResult() {
	have_result = true;
}

QueryResult::~QueryResult() {
	for (auto& result : results) {
		delete result;
	}
	results.clear();

}

bool QueryResult::haveResult() {
	return have_result;
}

void QueryResult::setNoResult() {
	have_result = false;
}

bool QueryResult::isInTables(std::vector<Entity> v) {
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

void QueryResult::addResult(ResultTable& t) {
	if (t.isEmpty()) {
		have_result = false;
		return;
	}
	//If new result table cannot be merge
	if (!isInTables(t.getHeaders())) {
		ResultTable* new_result = new ResultTable(t);
		results.push_back(new_result);
		addHeader(t.getHeaders());
		return;
	}
	addHeader(t.getHeaders());

	//Common header found, began merge
	ResultTable* holder = nullptr;
	auto& it = results.begin();
	while (it != results.end()) {
		bool merged = (*it)->merge(t);
		if (merged && (*it)->isEmpty()) {
			have_result = false;
			return;
		} else if (merged) {
			holder = *it;
			it++;
			break;
		} else {
			it++;
		}
	}

	while (it != results.end()) {
		bool merged = holder->merge(**it);
		if (merged && holder->isEmpty()) {
			have_result = false;
			return;
		} else if (merged) {
			delete *it;
			it = results.erase(it);
			break;
		} else {
			it++;
		}
	}
}

void QueryResult::addHeader(std::vector<Entity> v) {
	for (auto& it : v) {
		header_set.insert(it.getSynonym());
	}
}

void QueryResult::getResults(std::vector<Entity>& selected, ResultTable& out) {
	for (auto& result : results) {
		std::vector<Entity> common = result->getCommonHeaders(selected);
		if (common.empty()) {
			continue;
		}
		selected = Utility::getEntitiesExclude(selected, common);
		out.joinTable(result->getResultTable(common));
	}

}

void QueryResult::getResult(Entity e, std::list<std::string>& out) {
	for (auto& table : results) {
		if (table->isInTable(e)) {
			table->getEntityResult(e, out);
			return;
		}
	}
	throw std::domain_error("Invalid Entity, Source: QueryResult.getResult");
}
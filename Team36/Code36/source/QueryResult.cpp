#include "QueryResult.h"
#include <stdexcept>
#include "Utility.h"
#include <algorithm>

QueryResult::QueryResult() {
	have_result = true;
}

QueryResult::~QueryResult() {
	for (auto& result : results) {
		delete result.second;
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
	return header_set.find(e.getSynonym()) != header_set.end();
}
ResultTable* QueryResult::generateNewResultTable(ResultTable& t) {
	ResultTable* new_table = new ResultTable(t);
	addHeader(t.getHeaders());
	return new_table;
}

bool QueryResult::tryMergeResultTableCommonHeader(ResultTable& t, std::list<ResultTable*>& affected) {
	auto& it = results.find(t.getHeadersName());
	if (it == results.end()) {
		return false;
	}

	if (mergeResultTable(it->second, t)) {
		affected.emplace_back(it->second);
		return true;
	} else {
		throw std::exception("mergeResultTable() : merging should be successful");
	}
}

bool QueryResult::tryMergeResultTableIntoCurrentTable(ResultTable& t, std::list<ResultTable*>& affected) {
	auto& it = results.begin();
	while (it != results.end()) {
		if (mergeResultTable(it->second, t)) {
			affected.emplace_back(it->second);
			return true;
		}
		it++;
	}
	throw std::domain_error("There should exist duplicate column when this method is called");
}

void QueryResult::mergeResultTableWithNewTable(ResultTable& t, std::list<ResultTable*>& affected) {
	ResultTable* new_table = generateNewResultTable(t);
	auto& it = results.begin();
	while (it != results.end()) {
		if (mergeResultTable(new_table, *it->second)) {
			delete it->second;
			it = results.erase(it);
		} else {
			it++;
		}
	}

	results.insert({ new_table->getHeadersName(), new_table });
	affected.emplace_back(new_table);
}

bool QueryResult::mergeResultTable(ResultTable* table, ResultTable& to_merge) {
	bool merged = table->merge(to_merge);
	if (table->isEmpty()) {
		have_result = false;
	}
	return merged;
}

void QueryResult::mergeResultTable(ResultTable& t, std::list<ResultTable*>& affected) {
	if (tryMergeResultTableCommonHeader(t, affected)) {
		return;
	}else if (t.getHeaders().size() == 1 && tryMergeResultTableCommonHeader(t, affected)) {
		return;
	}else{
		mergeResultTableWithNewTable(t, affected);
	}
}

void QueryResult::getMergedTable(std::vector<Entity> selected, std::list<ResultTable>& out) {
	
	out.emplace_back(ResultTable());
	std::list<ResultTable*> result_list;
	for (auto& it = results.begin(); it != results.end(); ++it) {
		result_list.emplace_back(it->second);
	}

	bool selected_list_updated = true;
	while (selected_list_updated) {
		selected_list_updated = false;

		auto& it = result_list.begin();
		while (it != result_list.end()) {
			std::vector<Entity> common = (*it)->getCommonHeaders(selected);
			if (common.empty()) {
				it++;
				continue;
			}

			std::vector<Entity> new_selected = Utility::getEntitiesExclude((*it)->getHeaders(), common);
			for (auto& e : new_selected) {
				selected_list_updated = true;
				selected.emplace_back(e);
			}
			out.front().joinTable(**it);
			it = result_list.erase(it);
		}
	}

}


void QueryResult::addResult(ResultTable& t) {
	if (t.isEmpty()) {
		have_result = false;
		return;
	}
	//If new result table cannot be merge
	if (!isInTables(t.getHeaders())) {
		ResultTable* new_table = generateNewResultTable(t);
		results.insert({ new_table->getHeadersName(), new_table });
		return;
	}

	std::list<ResultTable*> affected;
	mergeResultTable(t, affected);
	if (!have_result) {
		return;
	}

	updateAllAffectedTable(affected);
}

void QueryResult::updateAllAffectedTable(std::list<ResultTable*>& affected) {

	std::unordered_set<std::string> affected_name;
	for (auto& rt : affected) {
		affected_name.insert(rt->getHeadersName());
	}

	while (!affected.empty()) {
		ResultTable* current = affected.front();
		affected.pop_front();
		affected_name.erase(current->getHeadersName());

		size_t current_original_size = current->size();

		auto& it = results.begin();
		while (it != results.end()) {
			if (it->second->getHeadersName() == current->getHeadersName()) {
				it++;
				continue;
			}
			size_t original_size = it->second->size();
			bool filtered = it->second->filter(*current);
			if (it->second->isEmpty() || current->isEmpty()) {
				have_result = false;
				return;
			} else if (filtered && original_size > it->second->size() &&
				affected_name.find(it->second->getHeadersName()) == affected_name.end()) {
				affected_name.insert(it->second->getHeadersName());
				affected.push_back(it->second);
			}
			it++;
		}
		if (current_original_size > current->size() &&
			affected_name.find(current->getHeadersName()) == affected_name.end()) {
			affected_name.insert(current->getHeadersName());
			affected.push_back(current);
		}
	}
}
void QueryResult::addHeader(std::vector<Entity> v) {
	for (auto& it : v) {
		header_set.insert(it.getSynonym());
	}
}

void QueryResult::getResults(std::vector<Entity>& selected, ResultTable& out) {
	std::list<ResultTable> merged_table;
	getMergedTable(selected, merged_table);

	for (auto& result : merged_table) {
		std::vector<Entity> common = result.getCommonHeaders(selected);
		if (common.empty()) {
			continue;
		}
		selected = Utility::getEntitiesExclude(selected, common);
		out.joinTable(result.getResultTable(common));
	}
}

void QueryResult::getResult(Entity e, std::list<std::string>& out) {
	for (auto& table : results) {
		if (table.second->isInTable(e)) {
			table.second->getEntityResult(e, out);
			return;
		}
	}
	throw std::domain_error("Invalid Entity, Source: QueryResult.getResult");
}


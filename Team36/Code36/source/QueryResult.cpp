#include "QueryResult.h"
#include <stdexcept>
#include "Utility.h"
#include <algorithm>
#include "QueryTupleOptimizer.h"

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

void QueryResult::getSelectedEntitiesMergedTable(std::vector<Entity> selected, std::list<ResultTable>& out) {


	for (auto& it = results.begin(); it != results.end(); ++it) {
		if (it->second->getHeaders().size() == 1 && it->second->getCommonHeaders(selected).size() == 1) {
			out.emplace_back(*it->second);
		}
	}

	QueryTupleOptimizer optimizer;
	std::list<std::pair<Entity, Entity>> headers = getAllTableHeaderWithTwoSynonym();
	std::list<std::list<std::pair<Entity, Entity>>> grouped_header = optimizer.groupCommonEntities(headers);

	for (auto& group : grouped_header) {
		std::vector<Entity> group_entity_list = Utility::getEntityListFromPair(group);
		std::vector<Entity> list_selected_group = Utility::getEntitiesInclude(selected, group_entity_list);
		if (list_selected_group.empty()) {
			continue;
		}

		out.emplace_back(ResultTable());
		joinResultTables(group, list_selected_group, out.back());
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


bool QueryResult::joinResultTables(std::list<std::pair<Entity, Entity>>& table_names) {
	return joinResultTables(table_names, {}, ResultTable());
}

bool QueryResult::joinResultTables(std::list<std::pair<Entity, Entity>>& table_names, std::vector<Entity> selected_list, ResultTable& out) {
	Entity selected = Utility::getEntityNameWithLeastFrequency(table_names);
	
	ResultTable result;
	while (!table_names.empty()) {
		std::list<std::pair<Entity, Entity>> selected_table = Utility::splitEntityPairs(table_names, selected);
		for (auto& selected : selected_table) {
			if (joinResultTable(result, selected)) {
				continue;
			}
			return false;
		}

		std::vector<Entity> remaining_entity_to_keep = Utility::getEntityListFromPair(table_names);
		remaining_entity_to_keep.insert(remaining_entity_to_keep.end(), selected_list.begin(), selected_list.end());
		std::vector<Entity> table_headers_without_unnecessary_header = Utility::getEntitiesInclude(result.getHeaders(), remaining_entity_to_keep);

		if (table_headers_without_unnecessary_header.size() == 0) {
			result = ResultTable();
		} else if (table_headers_without_unnecessary_header.size() < result.getHeaders().size()) {
			result = result.getResultTable(table_headers_without_unnecessary_header);
		}

		if (table_names.empty()) {
			break;
		}

		std::vector<Entity> common_entity = Utility::getEntitiesInclude(result.getHeaders(), Utility::getEntityListFromPair(table_names));
		if (common_entity.empty()) {
			selected = Utility::getEntityNameWithLeastFrequency(table_names);
		} else {
			selected = Utility::getEntityNameWithLeastFrequency(table_names, table_headers_without_unnecessary_header);
		}
	}

	if (!selected_list.empty()) {
		out.joinTable(result);
	}
	return true;

}

bool QueryResult::joinResultTable(ResultTable& out, std::pair<Entity, Entity> to_join_header) {
	std::string header_name = Utility::getSortedEntityName(to_join_header);
	out.joinTable(*results[header_name]);
	if (out.isEmpty()) {
		have_result = false;
		return false;
	}
	return true;
}

void QueryResult::addHeader(std::vector<Entity> v) {
	for (auto& it : v) {
		header_set.insert(it.getSynonym());
	}
}

std::list<std::pair<Entity, Entity>> QueryResult::getAllTableHeaderWithTwoSynonym() {
	std::list<std::pair<Entity, Entity>> headers_pair;

	auto& it = results.begin();
	while (it != results.end()) {
		std::vector<Entity> headers = it->second->getHeaders();
		if (headers.size() > 1) {
			headers_pair.push_back({ headers[0], headers[1]});
		}
		it++;
	}

	return headers_pair;
}

void QueryResult::getResults(std::vector<Entity>& selected, ResultTable& out) {
	std::list<ResultTable> merged_table;
	getSelectedEntitiesMergedTable(selected, merged_table);

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
	std::list<ResultTable> merged_table;
	getSelectedEntitiesMergedTable({ e }, merged_table);
	for (auto& table : merged_table) {
		if (table.isInTable(e)) {
			table.getEntityResult(e, out);
			return;
		}
	}
	throw std::domain_error("Invalid Entity, Source: QueryResult.getResult");
}

void QueryResult::updateHaveResultAfterTableJoin() {
	QueryTupleOptimizer optimizer;
	std::list<std::pair<Entity, Entity>> headers = getAllTableHeaderWithTwoSynonym();
	std::list<std::list<std::pair<Entity, Entity>>> grouped_header = optimizer.groupCommonEntities(headers);

	for (auto& group : grouped_header) {
		if (joinResultTables(group)) {
			continue;
		}
		break;
	}

}


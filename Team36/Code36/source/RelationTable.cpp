#include "RelationTable.h"

bool RelationTable::insert(stmt_index key, stmt_index value)
{
	auto iter = table.find(key);
	if (uniquePair) {
		// only add if key is not in table
		if (iter == table.end()) {
			table.emplace(key, std::vector{ value });
			return true;
		}
		else {
			return false;
		}
	}
	else {
		// only add if key-value pair is unique
		if (iter != table.end()) {
			table.emplace(key, std::vector<stmt_index>(value));
		}
		else {
			(iter->second).push_back(value);
		}
	}
}

std::vector<stmt_index> RelationTable::getValues(stmt_index key) {
	auto iter = table.find(key);
	return iter->second;
}

bool RelationTable::contains(stmt_index key)
{
	auto iter = table.find(key);
	return iter != table.end();
}

bool RelationTable::contains(stmt_index key, stmt_index value)
{
	auto iter = table.find(key);
	auto v = iter->second;
	return iter != table.end() && std::find(v.begin(), v.end(), value) != v.end();
}

RelationTable RelationTable::findTransitiveClosure()
{
	return RelationTable();
}

RelationTable RelationTable::findReverse()
{
	return RelationTable();
}
#include "RelationTable.h"

RelationTable::RelationTable()
{
}

bool RelationTable::insert(stmt_index key, stmt_index value)
{
	auto iter = table.find(key);
	if (uniqueKey) {
		// only add if key is not in table
		if (iter == table.end()) {
			table.emplace(key, std::vector<stmt_index>(value));
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
			return true;
		}
		else {
			auto v = iter->second;
			if (std::find(v.begin(), v.end(), value) == v.end()) {
				(iter->second).push_back(value);
				return true;
			}
			else {
				return false;
			}
		}
	}
}

std::vector<stmt_index> RelationTable::getValues(stmt_index key) {
	auto iter = table.find(key);
	return iter->second;
}

bool RelationTable::containsKey(stmt_index key)
{
	auto iter = table.find(key);
	return iter != table.end();
}

bool RelationTable::containsPair(stmt_index key, stmt_index value)
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
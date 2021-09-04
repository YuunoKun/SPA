#include "RelationTable.h"

RelationTable::RelationTable()
{
}

std::unordered_map<stmt_index, std::vector<stmt_index>> RelationTable::getTable()
{
	return table;
}

bool RelationTable::isUniqueKey()
{
	return uniqueKey;
}

bool RelationTable::insert(stmt_index key, stmt_index value)
{
	if ((key == 0) | (value == 0)) {
		throw std::invalid_argument("Received stmt_index of 0");
	}
	auto iter = table.find(key);
	if (uniqueKey) {
		// only add if key is not in table
		if (iter == table.end()) {
			std::vector<stmt_index> newV;
			newV.push_back(value);
			table.emplace(key, newV);
			return true;
		}
		else {
			return false;
		}
	}
	else {
		// only add if key-value pair is unique
		if (iter == table.end()) {
			std::vector<stmt_index> newV;
			newV.push_back(value);
			table.emplace(key, newV);
			return true;
		}
		else {
			auto v = iter->second;
			// only add if key-value pair does not already exist
			if (std::find(v.begin(), v.end(), value) != v.end()) {
				return false;
			}
			else {
				table[key].push_back(value);
				return true;
			}
		}
	}
}

std::vector<stmt_index> RelationTable::getValues(stmt_index key) {
	auto iter = table.find(key);
	if (iter != table.end()) {
		return iter->second;
	}
	else {
		return std::vector<stmt_index> {};
	}
}

std::vector<stmt_index> RelationTable::getKeys()
{
	std::vector<stmt_index> keys;
	keys.reserve(table.size());

	for (auto kv : table) {
		keys.push_back(kv.first);
	}
	return keys;
}

bool RelationTable::containsKey(stmt_index key)
{
	auto iter = table.find(key);
	return iter != table.end();
}

bool RelationTable::containsPair(stmt_index key, stmt_index value)
{
	auto iter = table.find(key);
	auto v = getValues(key);
	return containsKey(key) && std::find(v.begin(), v.end(), value) != v.end();
}

RelationTable RelationTable::findTransitiveClosure()
{
	return RelationTable();
}

RelationTable RelationTable::findReverse()
{
	RelationTable reversed = RelationTable(uniqueKey);
	for (auto const& pair : table) {
		for (auto const& value : pair.second)
			reversed.insert(value, pair.first);
	};
	return reversed;
}

bool RelationTable::operator==(const RelationTable& other_table) const {
	return table == other_table.table;
}

bool RelationTable::operator!= (const RelationTable& other_table) const {
	return table != other_table.table;
}
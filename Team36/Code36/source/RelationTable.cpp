#include "RelationTable.h"

std::unordered_map<int, std::vector<int>> RelationTable::getTable()
{
	return table;
}

bool RelationTable::isUniqueKey()
{
	return uniqueKey;
}

bool RelationTable::insert(int key, int value)
{
	if ((key == 0) | (value == 0)) {
		throw std::invalid_argument("Received int of 0");
	}
	auto iter = table.find(key);
	if (uniqueKey) {
		// only add if key is not in table
		if (iter == table.end()) {
			std::vector<int> newV;
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
			std::vector<int> newV;
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

std::vector<int> RelationTable::getValues(int key) {
	auto iter = table.find(key);
	if (iter != table.end()) {
		return iter->second;
	}
	else {
		return std::vector<int> {};
	}
}

std::vector<int> RelationTable::getKeys()
{
	std::vector<int> keys;
	keys.reserve(table.size());

	for (auto kv : table) {
		keys.push_back(kv.first);
	}
	return keys;
}

bool RelationTable::containsKey(int key)
{
	auto iter = table.find(key);
	return iter != table.end();
}

bool RelationTable::containsPair(int key, int value)
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
	RelationTable reversed;
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
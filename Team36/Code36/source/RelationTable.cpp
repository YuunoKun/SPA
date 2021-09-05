#pragma once

#include "RelationTable.h"

template <class T, class S>
std::unordered_map<T, std::vector<S>> RelationTable<T, S>::getTable()
{
	return table;
}

template <class T, class S>
void RelationTable<T, S>::clear()
{
	table.clear();
	return;
}

template <class T, class S>
bool RelationTable<T, S>::isUniqueKey()
{
	return uniqueKey;
}

template <class T, class S>
bool RelationTable<T, S>::insert(T key, S value)
{
	auto iter = table.find(key);
	// only add if key-value pair is unique
	if (iter == table.end()) {
		std::vector<S> newV;
		newV.push_back(value);
		table.emplace(key, newV);
		return true;
	}
	else {
		auto v = iter->second;
		if (std::find(v.begin(), v.end(), value) != v.end()) {
			return false;
		}
		else {
			table[key].push_back(value);
			return true;
		}
	}
}

template <class T, class S>
std::vector<S> RelationTable<T, S>::getValues(T key) {
	auto iter = table.find(key);
	if (iter != table.end()) {
		return iter->second;
	}
	else {
		return std::vector<T> {};
	}
}

template <class T, class S>
std::vector<T> RelationTable<T, S>::getKeys()
{
	std::vector<T> keys;
	keys.reserve(table.size());

	for (auto kv : table) {
		keys.push_back(kv.first);
	}
	return keys;
}

template <class T, class S>
std::vector<std::pair<T, S>> RelationTable<T, S>::getPairs()
{
	std::vector<std::pair<T, T>> result;
	for (auto const& pair : table) {
		T key = pair.first;
		for (auto const& value : pair.second)
			result.push_back(std::make_pair(key, value));
	};
	return result;
}

template <class T, class S>
bool RelationTable <T, S>::containsKey(T key)
{
	auto iter = table.find(key);
	return iter != table.end();
}

template <class T, class S>
bool RelationTable<T, S>::containsPair(T key, S value)
{
	auto iter = table.find(key);
	auto v = getValues(key);
	return containsKey(key) && std::find(v.begin(), v.end(), value) != v.end();
}

template <class T, class S>
bool RelationTable<T, S>::isEmpty()
{
	return table.empty();
}

template <class T, class S>
RelationTable<T, S> RelationTable<T, S>::findTransitiveClosure()
{
	return RelationTable();
}

template <class T, class S>
RelationTable<S, T> RelationTable<T, S>::findReverse()
{
	RelationTable<S, T> reversed;
	for (auto const& pair : table) {
		for (auto const& value : pair.second)
			reversed.insert(value, pair.first);
	};
	return reversed;
}

template <class T, class S>
bool RelationTable<T, S>::operator==(const RelationTable& other_table) const {
	return (uniqueKey == other_table.uniqueKey) && (table == other_table.table);
}

template <class T, class S>
bool RelationTable<T, S>::operator!=(const RelationTable& other_table) const {
	return (uniqueKey != other_table.uniqueKey) || (table != other_table.table);
}

template <class T, class S>
bool UniqueRelationTable<T, S>::insert(T key, S value)
{
	auto iter = table.find(key);
	// only add if key is not in table
	if (iter == table.end()) {
		std::vector<S> newV;
		newV.push_back(value);
		table.emplace(key, newV);
		return true;
	}
	else {
		return false;
	}
}
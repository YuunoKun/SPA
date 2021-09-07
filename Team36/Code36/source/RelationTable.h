#pragma once

#include<stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Common.h"

template <class T, class S>
class RelationTable {
public:
	RelationTable() {
		uniqueKey = false;
	};
	bool isUniqueKey();
	bool insert(T key, S value);
	std::vector<S> getValues(T);
	std::vector<T> getKeys();
	std::vector<std::pair<T, S>> getPairs();
	bool containsKey(T);
	bool containsPair(T, S);
	RelationTable<T, S> findTransitiveClosure();
	RelationTable<S, T> findReverse();
	void clear();
	bool isEmpty();

	bool operator==(const RelationTable&) const;
	bool operator!=(const RelationTable&) const;

protected:
	std::unordered_map<T, std::vector<S>> table;
	bool uniqueKey;

	std::unordered_map<T, std::vector<S>> getTable();
};

template <class T, class S>
class UniqueRelationTable : public RelationTable<T, S> {
public:
	UniqueRelationTable() {
		uniqueKey = true;
	};
	bool insert(T key, S value);
};

template <class T, class S>
inline
std::unordered_map<T, std::vector<S>> RelationTable<T, S>::getTable() {
	return table;
}

template <class T, class S>
inline
void RelationTable<T, S>::clear() {
	table.clear();
	return;
}

template <class T, class S>
inline
bool RelationTable<T, S>::isUniqueKey() {
	return uniqueKey;
}

template <class T, class S>
inline
bool RelationTable<T, S>::insert(T key, S value) {
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
inline
std::vector<S> RelationTable<T, S>::getValues(T key) {
	auto iter = table.find(key);
	if (iter != table.end()) {
		return iter->second;
	}
	else {
		return std::vector<S> {};
	}
}

template <class T, class S>
inline
std::vector<T> RelationTable<T, S>::getKeys() {
	std::vector<T> keys;
	keys.reserve(table.size());

	for (auto kv : table) {
		keys.push_back(kv.first);
	}
	return keys;
}

template <class T, class S>
inline
std::vector<std::pair<T, S>> RelationTable<T, S>::getPairs() {
	std::vector<std::pair<T, S>> result;
	for (auto const& pair : table) {
		T key = pair.first;
		for (auto const& value : pair.second)
			result.push_back(std::make_pair(key, value));
	};
	return result;
}

template <class T, class S>
inline
bool RelationTable <T, S>::containsKey(T key) {
	auto iter = table.find(key);
	return iter != table.end();
}

template <class T, class S>
inline
bool RelationTable<T, S>::containsPair(T key, S value) {
	auto iter = table.find(key);
	auto v = getValues(key);
	return containsKey(key) && std::find(v.begin(), v.end(), value) != v.end();
}

template <class T, class S>
inline
bool RelationTable<T, S>::isEmpty() {
	return table.empty();
}

template <class T, class S>
inline
RelationTable<T, S> RelationTable<T, S>::findTransitiveClosure() {
	return RelationTable();
}

template <class T, class S>
inline
RelationTable<S, T> RelationTable<T, S>::findReverse() {
	RelationTable<S, T> reversed;
	for (auto const& pair : table) {
		for (auto const& value : pair.second)
			reversed.insert(value, pair.first);
	};
	return reversed;
}

template <class T, class S>
inline
bool RelationTable<T, S>::operator==(const RelationTable& other_table) const {
	return (uniqueKey == other_table.uniqueKey) && (table == other_table.table);
}

template <class T, class S>
inline
bool RelationTable<T, S>::operator!=(const RelationTable& other_table) const {
	return (uniqueKey != other_table.uniqueKey) || (table != other_table.table);
}

template <class T, class S>
inline
bool UniqueRelationTable<T, S>::insert(T key, S value) {
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

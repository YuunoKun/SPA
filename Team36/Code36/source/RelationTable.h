#pragma once

#include<stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Common.h"

template <class T>
class RelationTable {
public:
	RelationTable() {
		uniqueKey = false;
	};
	bool isUniqueKey();
	bool insert(T key, T value);
	std::vector<T> getValues(T);
	std::vector<T> getKeys();
	std::vector<std::pair<T, T>> getPairs();
	bool containsKey(T);
	bool containsPair(T, T);
	RelationTable findTransitiveClosure();
	RelationTable findReverse();
	void clear();

	bool operator==(const RelationTable&) const;
	bool operator!=(const RelationTable&) const;

protected:
	std::unordered_map<T, std::vector<T>> table;
	bool uniqueKey;

	std::unordered_map<T, std::vector<T>> getTable();
};

template <class T>
class UniqueRelationTable : public RelationTable<T> {
public:
	UniqueRelationTable() {
		uniqueKey = true;
	};
	bool insert(T key, T value);
};
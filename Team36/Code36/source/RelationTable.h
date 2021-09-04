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
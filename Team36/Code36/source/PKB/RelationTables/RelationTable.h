#pragma once

#include<stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Common/Common.h"

template <class T, class S>
class RelationTable {
public:
	RelationTable() {};
	RelationTable(std::vector<std::pair<T, S>> input) {
		for (auto& pair : input) {
			insert(pair.first, pair.second);
		}
	};
	void clear();
	bool isEmpty() const;
	bool insert(T key, S value);
	std::vector<T> getKeys(S) const;
	std::vector<S> getValues(T) const;
	std::vector<T> getKeys() const;
	std::vector<S> getValues() const;
	std::vector<std::pair<T, S>> getPairs() const;
	bool containsKey(T) const;
	bool containsValue(S) const;
	bool containsPair(T, S) const;

	RelationTable<T, S> copy() const;

	std::unordered_map<T, std::vector<S>> getTableForward();
	std::unordered_map<S, std::vector<T>> getTableBackward();
	void sort();

	bool operator==(const RelationTable&) const;
	bool operator!=(const RelationTable&) const;

protected:
	std::unordered_map<T, std::vector<S>> forward_table;
	std::unordered_map<S, std::vector<T>> backward_table;
	std::unordered_map<T, std::unordered_set<S>> lookup_table;
};
#pragma once

#include<stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Common.h"

class RelationTable {
public:
	RelationTable() {
		uniqueKey = false;
	};
	bool isUniqueKey();
	bool insert(int key, int value);
	std::vector<int> getValues(int);
	std::vector<int> getKeys();
	bool containsKey(int);
	bool containsPair(int, int);
	RelationTable findTransitiveClosure();
	RelationTable findReverse();

	bool operator==(const RelationTable&) const;
	bool operator!=(const RelationTable&) const;

protected:
	std::unordered_map<int, std::vector<int>> table;
	bool uniqueKey;

	std::unordered_map<int, std::vector<int>> getTable();
};

class UniqueRelationTable : public RelationTable {
public:
	UniqueRelationTable() {
		uniqueKey = true;
	};
};
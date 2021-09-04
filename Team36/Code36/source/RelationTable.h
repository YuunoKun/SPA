#pragma once

#include<stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Common.h"

class RelationTable {
public:
	RelationTable(bool uniqueKey) {
		this->uniqueKey = uniqueKey;
	};
	bool isUniqueKey();
	bool insert(stmt_index key, stmt_index value);
	std::vector<stmt_index> getValues(stmt_index);
	std::vector<stmt_index> getKeys();
	bool containsKey(stmt_index);
	bool containsPair(stmt_index, stmt_index);
	RelationTable findTransitiveClosure();
	RelationTable findReverse();

	bool operator==(const RelationTable&) const;
	bool operator!=(const RelationTable&) const;

private:
	std::unordered_map<stmt_index, std::vector<stmt_index>> table;
	bool uniqueKey = true;

	RelationTable();
	std::unordered_map<stmt_index, std::vector<stmt_index>> getTable();
};

#pragma once

#include<stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Common.h"

class RelationTable {
public:
	RelationTable(bool uniquePair) {
		this->uniquePair = uniquePair;
	};
	RelationTable();
	bool insert(stmt_index key, stmt_index value);
	std::vector<stmt_index> getValues(stmt_index);
	bool isUniqueKeys();
	bool contains(stmt_index);
	bool contains(stmt_index, stmt_index);
	RelationTable findTransitiveClosure();
	RelationTable findReverse();

private:
	std::unordered_map<stmt_index, std::vector<stmt_index>> table;
	bool uniquePair;
};

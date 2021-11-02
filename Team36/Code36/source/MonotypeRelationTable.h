#pragma once

#include<stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Common.h"
#include "RelationTable.h"
#include "RelationTable.cpp"

template <class T>
class MonotypeRelationTable : public RelationTable<T, T> {
public:
	MonotypeRelationTable() {};
	MonotypeRelationTable(std::vector<std::pair<T, T>> input) {
		for (auto& pair : input) {
			insert(pair.first, pair.second);
		}
	};

protected:
	std::unordered_set<T> calculated_dfs_forward;
	std::unordered_set<T> calculated_dfs_backward;
};

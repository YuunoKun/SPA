#pragma once

#include<stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Common/Common.h"
#include "MonotypeRelationTable.h"

template <class T>
class RelationTableUtility {
public:
	static std::vector<T> forwardDFS(const MonotypeRelationTable<T>& table, T start);
	static std::vector<T> backwardDFS(const MonotypeRelationTable<T>& table, T start);

	static MonotypeRelationTable<T> findTransitiveClosure(const MonotypeRelationTable<T>& table);
	static MonotypeRelationTable<T> copy(const MonotypeRelationTable<T>& table);
private:
	static  std::vector<T> transitiveClosureDFS(MonotypeRelationTable<T>& table, T start, std::unordered_set<T>& calculated_list);
};
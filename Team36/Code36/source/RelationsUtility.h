#pragma once

#include<stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Common.h"
#include "RelationTable.h"

template <class T>
class RelationsUtility {
public:
	static std::vector<T> forwardDFS(const RelationTable<T, T>& table, T start);
	static std::vector<T> backwardDFS(const RelationTable<T, T>& table, T start);

	static RelationTable<T, T> findTransitiveClosure(const RelationTable<T, T>& table);
};

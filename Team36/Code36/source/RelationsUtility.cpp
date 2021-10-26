#pragma once

#include<stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Common.h"
#include "RelationsUtility.h"

template<class T>
std::vector<T> RelationsUtility<T>::forwardDFS(const RelationTable<T, T>& table, T start) {
	std::vector<T> res;

	std::unordered_set<T> visited;
	std::stack<T> stack;

	for (auto& it : table.getValues(start)) {
		stack.push(it);
	}

	while (!stack.empty()) {
		T s = stack.top();
		stack.pop();

		if (!visited.count(s)) {
			visited.emplace(s);
			res.push_back(s);
		}

		for (auto it : table.getValues(s)) {
			if (!visited.count(it)) {
				stack.push(it);
			}
		}
	}
	std::sort(res.begin(), res.end());
	return res;
}

template<class T>
std::vector<T> RelationsUtility<T>::backwardDFS(const RelationTable<T, T>& table, T start) {
	std::vector<T> res;

	std::unordered_set<T> visited;
	std::stack<T> stack;

	for (auto it : table.getKeys(start)) {
		stack.push(it);
	}

	while (!stack.empty()) {
		T s = stack.top();
		stack.pop();

		if (!visited.count(s)) {
			visited.emplace(s);
			res.push_back(s);
		}

		for (auto it : table.getKeys(s)) {
			if (!visited.count(it)) {
				stack.push(it);
			}
		}
	}
	std::sort(res.begin(), res.end());
	return res;
}

template<class T>
RelationTable<T, T> RelationsUtility<T>::findTransitiveClosure(const RelationTable<T, T>& table) {
	RelationTable<T, T> res = table.copy();
	for (T& k : table.getKeys()) {
		for (T& vertice : forwardDFS(table, k)) {
			res.insert(k, vertice);
		}
	}
	res.sort();
	return res;
}
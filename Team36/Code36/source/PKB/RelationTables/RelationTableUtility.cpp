#pragma once

#include <stdio.h>

#include <iostream>
#include "RelationTableUtility.h"

template<class T>
std::vector<T> RelationTableUtility<T>::forwardDFS(const MonotypeRelationTable<T>& table, T start) {
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
std::vector<T> RelationTableUtility<T>::backwardDFS(const MonotypeRelationTable<T>& table, T start) {
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
	return res;
}

template<class T>
std::vector<T> RelationTableUtility<T>::transitiveClosureDFS(MonotypeRelationTable<T>& table, T start, std::unordered_set<T>& calculated_list) {
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
		if (calculated_list.count(s)) {
			for (auto it : table.getValues(s)) {
				res.push_back(it);
			}
			continue;
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
MonotypeRelationTable<T> RelationTableUtility<T>::findTransitiveClosure(const MonotypeRelationTable<T>& table) {
	std::unordered_set<T> calculated_dfs_forward;
	MonotypeRelationTable<T> res = copy(table);
	std::vector<T> keys = res.getKeys();
	if (keys.size() == 0) {
		return res;
	}

	std::reverse(keys.begin(), keys.end());
	for (T& k : keys) {
		std::vector<T> dfs_key = transitiveClosureDFS(res, k, calculated_dfs_forward);
		for (T& value : dfs_key) {
			res.insert(k, value);
		}
		calculated_dfs_forward.emplace(k);
	}
	res.sort();
	return res;
}

template<class T>
MonotypeRelationTable<T> RelationTableUtility<T>::copy(const MonotypeRelationTable<T>& table) {
	MonotypeRelationTable<T> res;
	for (auto const& pair : table.getPairs()) {
		T key = pair.first;
		T value = pair.second;
		res.insert(key, value);
	}
	return res;
}
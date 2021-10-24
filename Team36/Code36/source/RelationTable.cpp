#pragma once
#include <iostream>
#include <queue>
#include "RelationTable.h"
#include <stack>

template <class T, class S>
std::unordered_map<T, std::vector<S>> RelationTable<T, S>::getTableForward() {
	return forward_table;
}

template <class T, class S>
std::unordered_map<S, std::vector<T>> RelationTable<T, S>::getTableBackward() {
	return backward_table;
}

template <class T, class S>
bool RelationTable<T, S>::isUniqueKey() const {
	return uniqueKey;
}

template <class T, class S>
void RelationTable<T, S>::clear() {
	forward_table.clear();
	backward_table.clear();
	lookup_table.clear();
	return;
}

template <class T, class S>
bool RelationTable<T, S>::isEmpty() const {
	return forward_table.empty();
}

template <class T, class S>
bool RelationTable<T, S>::insert(T key, S value) {
	auto iter_forward = forward_table.find(key);
	auto iter_backward = backward_table.find(value);
	bool keyExistsForward = iter_forward != forward_table.end();
	bool valueExistsBackward = iter_backward != backward_table.end();

	if (keyExistsForward) {
		auto v = iter_forward->second;
		bool keyValuePairExists = std::find(v.begin(), v.end(), value) != v.end();
		if (keyValuePairExists) {
			return false;
		}
	}

	if (keyExistsForward) {
		forward_table[key].push_back(value);
		lookup_table[key].emplace(value);
	}
	else {
		std::vector<S> new_forward_v;
		new_forward_v.push_back(value);
		forward_table.emplace(key, new_forward_v);

		std::unordered_set<S> new_set({ value });;
		lookup_table.emplace(key, new_set);
	}

	if (valueExistsBackward) {
		backward_table[value].push_back(key);
	}
	else {
		std::vector<T> new_backward_v;
		new_backward_v.push_back(key);
		backward_table.emplace(value, new_backward_v);
	}
	return true;
}

template<class T, class S>
std::vector<T> RelationTable<T, S>::getKeys(S value) {
	auto iter = backward_table.find(value);
	if (iter != backward_table.end()) {
		return iter->second;
	}
	else {
		return std::vector<T> {};
	}
}

template <class T, class S>
std::vector<S> RelationTable<T, S>::getValues(T key) {
	auto iter = forward_table.find(key);
	if (iter != forward_table.end()) {
		return iter->second;
	}
	else {
		return std::vector<S> {};
	}
}

template <class T, class S>
std::vector<T> RelationTable<T, S>::getKeys() const {
	std::vector<T> keys;
	keys.reserve(forward_table.size());

	for (auto kv : forward_table) {
		keys.push_back(kv.first);
	}
	return keys;
}

template<class T, class S>
std::vector<S> RelationTable<T, S>::getValues() const {
	std::vector<S> values;
	values.reserve(backward_table.size());

	for (auto kv : backward_table) {
		values.push_back(kv.first);
	}
	return values;
}

template <class T, class S>
std::vector<std::pair<T, S>> RelationTable<T, S>::getPairs() const {
	std::vector<std::pair<T, S>> result;
	for (auto const& pair : forward_table) {
		T key = pair.first;
		for (auto const& value : pair.second)
			result.push_back(std::make_pair(key, value));
	};
	return result;
}

template <class T, class S>
bool RelationTable <T, S>::containsKey(T key) const {
	auto iter = forward_table.find(key);
	return iter != forward_table.end();
}

template<class T, class S>
bool RelationTable<T, S>::containsValue(S value) const {
	auto iter = backward_table.find(value);
	return iter != backward_table.end();
}

template <class T, class S>
bool RelationTable<T, S>::containsPair(T key, S value) {
	auto s = lookup_table[key];
	return containsKey(key) && s.find(value) != s.end();
}

template<class T, class S>
std::vector<S> RelationTable<T, S>::forwardDFS(T key) {
	static_assert(std::is_same<T, S>::value, "DFS must be used with a table with columns of same datatype");
	if (calculated_dfs_forward.count(key)) {
		return getValues(key);
	}
	std::unordered_set<T> visited;
	std::stack<T> stack;

	for (auto it : getValues(key)) {
		stack.push(it);
	}

	while (!stack.empty()) {
		T s = stack.top();
		stack.pop();
		if (calculated_dfs_forward.count(s)) {
			visited.emplace(s);
			for (auto it : getValues(s)) {
				insert(key, it);
			}
		}
		if (!visited.count(s)) {
			visited.emplace(s);
			insert(key, s);
		}

		for (auto it : getValues(s)) {
			if (!visited.count(it)) {
				stack.push(it);
			}
		}
	}
	calculated_dfs_forward.emplace(key);
	return getValues(key);
}

template<class T, class S>
std::vector<T> RelationTable<T, S>::backwardDFS(S value) {
	static_assert(std::is_same<T, S>::value, "DFS must be used with a table with columns of same datatype");
	if (calculated_dfs_backward.count(value)) {
		return getValues(value);
	}
	std::unordered_set<S> visited;
	std::stack<S> stack;

	for (auto it : getKeys(value)) {
		stack.push(it);
	}

	while (!stack.empty()) {
		S s = stack.top();
		stack.pop();
		if (calculated_dfs_backward.count(s)) {
			visited.emplace(s);
			for (auto it : getKeys(s)) {
				insert(it, value);
			}
		}
		if (!visited.count(s)) {
			visited.emplace(s);
			insert(s, value);
		}

		for (auto it : getKeys(s)) {
			if (!visited.count(it)) {
				stack.push(it);
			}
		}
	}
	calculated_dfs_backward.emplace(value);
	return getKeys(value);
}

template <class T, class S>
RelationTable<T, S> RelationTable<T, S>::copy() const {
	RelationTable<T, S> res;
	for (auto const& pair : forward_table) {
		T key = pair.first;
		auto v1 = pair.second;
		for (auto const& value : v1) {
			res.insert(key, value);
		}
	}
	return res;
}

template <class T, class S>
RelationTable<T, S> RelationTable<T, S>::findTransitiveClosure() const {
	static_assert(std::is_same<T, S>::value, "Transitive closure must be used with a table with columns of same datatype");
	RelationTable<T, S> res = copy();
	std::vector<T> keys = res.getKeys();
	if (keys.size() == 0) {
		return res;
	}

	std::reverse(keys.begin(), keys.end());
	for (auto const& value : keys) {
		res.forwardDFS(value);
	}
	for (auto& pair : res.forward_table) {
		std::sort(pair.second.begin(), pair.second.end());
	}
	for (auto& pair : res.backward_table) {
		std::sort(pair.second.begin(), pair.second.end());
	}
	return res;
}

template <class T, class S>
bool RelationTable<T, S>::operator==(const RelationTable& other_table) const {
	return (uniqueKey == other_table.uniqueKey) && (forward_table == other_table.forward_table) && (backward_table == other_table.backward_table);
}

template <class T, class S>
bool RelationTable<T, S>::operator!=(const RelationTable& other_table) const {
	return (uniqueKey != other_table.uniqueKey) || (forward_table != other_table.forward_table) || (backward_table != other_table.backward_table);
}

template <class T, class S>
bool UniqueRelationTable<T, S>::insert(T key, S value) {
	auto iter_forward = forward_table.find(key);
	auto iter_backward = backward_table.find(value);
	bool keyExistsForward = iter_forward != forward_table.end();
	bool valueExistsBackward = iter_backward != backward_table.end();

	// only add if key is unique
	if (keyExistsForward) {
		return false;
	}
	else {
		std::vector<S> newV;
		newV.push_back(value);
		forward_table.emplace(key, newV);

		std::unordered_set<S> new_set({ value });;
		lookup_table.emplace(key, new_set);

		if (valueExistsBackward) {
			backward_table[value].push_back(key);
		}
		else {
			std::vector<T> new_backward_v;
			new_backward_v.push_back(key);
			backward_table.emplace(value, new_backward_v);
		}
		return true;
	}
}
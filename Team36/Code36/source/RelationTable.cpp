#pragma once
#include <iostream>
#include <queue>
#include "RelationTable.h"
#include <stack>
#include <unordered_map>

template <class T, class S>
std::unordered_map<T, std::vector<S>> RelationTable<T, S>::getTableForward() {
	return forward_table;
}

template <class T, class S>
std::unordered_map<S, std::vector<T>> RelationTable<T, S>::getTableBackward() {
	return backward_table;
}

template<class T, class S>
void RelationTable<T, S>::sort() {
	for (auto& pair : forward_table) {
		std::sort(pair.second.begin(), pair.second.end());
	}
	for (auto& pair : backward_table) {
		std::sort(pair.second.begin(), pair.second.end());
	}
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
std::vector<T> RelationTable<T, S>::getKeys(S value) const {
	auto iter = backward_table.find(value);
	if (iter != backward_table.end()) {
		return iter->second;
	}
	else {
		return std::vector<T> {};
	}
}

template <class T, class S>
std::vector<S> RelationTable<T, S>::getValues(T key) const {
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
bool RelationTable<T, S>::containsPair(T key, S value) const {
	if (containsKey(key)) {
		auto s = lookup_table.at(key);
		return s.find(value) != s.end();
	}
	return false;
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
bool RelationTable<T, S>::operator==(const RelationTable& other_table) const {
	return (forward_table == other_table.forward_table) && (backward_table == other_table.backward_table);
}

template <class T, class S>
bool RelationTable<T, S>::operator!=(const RelationTable& other_table) const {
	return (forward_table != other_table.forward_table) || (backward_table != other_table.backward_table);
}
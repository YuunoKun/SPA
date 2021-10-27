#include "RelationTable.cpp"
#include "MonotypeRelationTable.h"

template<class T>
std::vector<T> MonotypeRelationTable<T>::forwardDFS(T key) {
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

template<class T>
std::vector<T> MonotypeRelationTable<T>::backwardDFS(T value) {
	if (calculated_dfs_backward.count(value)) {
		return getValues(value);
	}
	std::unordered_set<T> visited;
	std::stack<T> stack;

	for (auto it : getKeys(value)) {
		stack.push(it);
	}

	while (!stack.empty()) {
		T s = stack.top();
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

template<class T>
MonotypeRelationTable<T> MonotypeRelationTable<T>::copy() const {
	MonotypeRelationTable<T> res;
	for (auto const& pair : forward_table) {
		T key = pair.first;
		auto v1 = pair.second;
		for (auto const& value : v1) {
			res.insert(key, value);
		}
	}
	return res;
}

template<class T>
MonotypeRelationTable<T> MonotypeRelationTable<T>::findTransitiveClosure() const {
	MonotypeRelationTable <T> res = copy();
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
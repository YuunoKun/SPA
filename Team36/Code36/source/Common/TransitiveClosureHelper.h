#pragma once
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <algorithm>

template <class T>
class TransitiveClosureHelper {
public:
	static std::vector<std::pair<T, T>> findTransitiveClosure(std::vector<std::pair<T, T>> input);
private:
	static std::vector<T> transitiveClosureDFS(std::unordered_map<T, std::set<T>>&, T start);
};

template<class T>
inline std::vector<std::pair<T, T>> TransitiveClosureHelper<T>::findTransitiveClosure(std::vector<std::pair<T, T>> input) {
	std::vector<std::pair<T, T>> res;
	std::unordered_map<T, std::set<T>> input_map;

	for (auto const& it : input) {
		input_map[it.first].emplace(it.second);
	}

	for (const auto& pair : input_map) {
		T k = pair.first;
		std::vector<T> dfs_val = transitiveClosureDFS(input_map, k);
		for (T value : dfs_val) {
			res.push_back({ k, value });
		}
	}
	return res;
}

template<class T>
inline std::vector<T> TransitiveClosureHelper<T>::transitiveClosureDFS(std::unordered_map<T, std::set<T>>& input, T start) {
	std::vector<T> res;

	std::unordered_set<T> visited;
	std::stack<T> stack;

	for (auto& val : input[start]) {
		stack.push(val);
	}

	while (!stack.empty()) {
		T s = stack.top();
		stack.pop();

		if (!visited.count(s)) {
			visited.emplace(s);
			res.push_back(s);
		}

		for (auto& val : input[s]) {
			if (!visited.count(val)) stack.push(val);
		}
	}
	std::sort(res.begin(), res.end());
	return res;
}
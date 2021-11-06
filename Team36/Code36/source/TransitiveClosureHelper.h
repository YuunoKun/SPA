#pragma once
#include <vector>
#include <unordered_set>
#include <stack>
#include <algorithm>

template <class T>
class TransitiveClosureHelper {
public:
	static std::vector<std::pair<T, T>> findTransitiveClosure(std::vector<std::pair<T, T>> input);
private:
	static std::vector<T> transitiveClosureDFS(std::vector<std::pair<T, T>> input, T start);
};

template<class T>
inline std::vector<std::pair<T, T>> TransitiveClosureHelper<T>::findTransitiveClosure(std::vector<std::pair<T, T>> input) {
	std::vector<std::pair<T, T>> res;
	std::unordered_set<T> keys;

	for (auto& it : input) {
		keys.emplace(it.first);
	}

	for (const T& k : keys) {
		std::vector<T> dfs_val = transitiveClosureDFS(input, k);
		for (T value : dfs_val) {
			res.push_back({ k, value });
		}
	}
	return res;
}

template<class T>
inline std::vector<T> TransitiveClosureHelper<T>::transitiveClosureDFS(std::vector<std::pair<T, T>> input, T start) {
	std::vector<T> res;

	std::unordered_set<T> visited;
	std::stack<T> stack;

	for (auto& it : input) {
		if (it.first == start && !visited.count(it.second)) {
			stack.push(it.second);
		}
	}

	while (!stack.empty()) {
		T s = stack.top();
		stack.pop();

		if (!visited.count(s)) {
			visited.emplace(s);
			res.push_back(s);
		}

		for (auto& it : input) {
			if (it.first == s && !visited.count(it.second)) {
				stack.push(it.second);
			}
		}
	}
	std::sort(res.begin(), res.end());
	return res;
}
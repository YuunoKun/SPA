#include "RelationTable.h"

template <class T>
std::unordered_map<T, std::vector<T>> RelationTable<T>::getTable()
{
	return table;
}

template <class T>
bool RelationTable<T>::isUniqueKey()
{
	return uniqueKey;
}

template <class T>
bool RelationTable<T>::insert(T key, T value)
{
	auto iter = table.find(key);
	// only add if key-value pair is unique
	if (iter == table.end()) {
		std::vector<T> newV;
		newV.push_back(value);
		table.emplace(key, newV);
		return true;
	}
	else {
		auto v = iter->second;
		if (std::find(v.begin(), v.end(), value) != v.end()) {
			return false;
		}
		else {
			table[key].push_back(value);
			return true;
		}
	}
}

template <class T>
std::vector<T> RelationTable<T>::getValues(T key) {
	auto iter = table.find(key);
	if (iter != table.end()) {
		return iter->second;
	}
	else {
		return std::vector<T> {};
	}
}

template <class T>
std::vector<T> RelationTable<T>::getKeys()
{
	std::vector<T> keys;
	keys.reserve(table.size());

	for (auto kv : table) {
		keys.push_back(kv.first);
	}
	return keys;
}

template<class T>
std::vector<std::pair<T, T>> RelationTable<T>::getPairs()
{
	std::vector<std::pair<T, T>> result;
	for (auto const& pair : table) {
		T key = pair.first;
		for (auto const& value : pair.second)
			result.push_back(std::make_pair(key, value));
	};
	return result;
}

template <class T>
bool RelationTable <T>::containsKey(T key)
{
	auto iter = table.find(key);
	return iter != table.end();
}

template <class T>
bool RelationTable<T>::containsPair(T key, T value)
{
	auto iter = table.find(key);
	auto v = getValues(key);
	return containsKey(key) && std::find(v.begin(), v.end(), value) != v.end();
}

template <class T>
RelationTable<T> RelationTable<T>::findTransitiveClosure()
{
	return RelationTable();
}

template <class T>
RelationTable<T> RelationTable<T>::findReverse()
{
	RelationTable reversed;
	for (auto const& pair : table) {
		for (auto const& value : pair.second)
			reversed.insert(value, pair.first);
	};
	return reversed;
}

template <class T>
bool RelationTable<T>::operator==(const RelationTable& other_table) const {
	return table == other_table.table;
}

template <class T>
bool RelationTable<T>::operator!= (const RelationTable& other_table) const {
	return table != other_table.table;
}

template <class T>
bool UniqueRelationTable<T>::insert(T key, T value)
{
	auto iter = table.find(key);
	// only add if key is not in table
	if (iter == table.end()) {
		std::vector<T> newV;
		newV.push_back(value);
		table.emplace(key, newV);
		return true;
	}
	else {
		return false;
	}
}
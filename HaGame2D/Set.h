#pragma once
#include <unordered_map>
#include <functional>

template <class T>
class Set {
	std::unordered_map<int, T> _set;
	std::function<int(T)> _hashFunc;

public:

	Set(std::function<int(T)> hashFunc) {
		_hashFunc = hashFunc;
	}

	bool exists(T item) {
		return _set.find(_hashFunc(item)) != _set.end();
	}

	void add(T item) {
		if (!exists(item)) {
			_set.insert(std::pair<int, T>(_hashFunc(item), item));
		}
	}

	void remove(T item) {
		if (exists(item)) {
			_set.erase(_hashFunc(item));
		}
	}

	std::vector<T> set() {
		std::vector<T> items;

		for (auto kv: _set) {
			items.push_back(kv.second);
		}

		return items;
	}

	void iterate(std::function<void(T)> iterFunc) {
		for (auto kv : _set) {
			iterFunc(kv.second);
		}
	}
};
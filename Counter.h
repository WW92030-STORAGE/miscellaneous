#ifndef COUNTER_H_NORMALEXISTING
#define COUNTER_H_NORMALEXISTING

#include <map>

template <typename T>
struct Counter {
	std::map<T, int> counter;

	bool contains(T t) {
		return counter.find(t) != counter.end();
	}

	int push(T t) {
		return ++counter[t];
	}

	int pop(T t) {
		auto it = counter.find(t);
		if (it == counter.end()) return -1;
		int retval = --(it->second);
		if (retval <= 0) counter.erase(it);
		return retval;
	}

	int count(T t) {
		auto it = counter.find(t);
		if (it != counter.end()) return it->second;
		return 0;
	}
};

#endif
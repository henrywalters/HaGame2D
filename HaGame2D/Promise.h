#pragma once
#include <functional>

template <class T>
class Resolution {

public:
	std::function<void(T)> resolveFunc;
	std::function<void()> rejectFunc;

	bool hasResolve, hasReject;

	//if and when the promise completes, resolveFn will be executed.
	Resolution<T> then(std::function<void(T)> resolveFn) {
		resolveFunc = resolveFn;
		hasResolve = true;
		return *this;
	}

	//if and when the promise fails, rejectFn will be executed.
	Resolution<T> except(std::function<void()> rejectFn) {
		rejectFunc = rejectFn;
		hasReject = true;
		return *this;
	}
};

template <class T>
class Promise
{
public:

	template <class T>
	void resolve(Resolution<T> resolution, T value) {
		if (resolution.hasResolve) {
			resolution.resolveFunc(value);
		}
	}

	template <class T>
	void reject(Resolution<T> resolution) {
		if (resolution.hasReject) {
			resolution.rejectFunc();
		}
	}


	Promise() {

	}

	~Promise() {

	}

};


#pragma once

template <class T>
struct QuickListNode {
	T id;
	QuickListNode * next;
};

template <class T>
class QuickList
{
private:
	QuickListNode<T> * root;
	QuickListNode<T> * tail;
	QuickListNode<T> * iter;
	int elements;
public:
	QuickList() {
		root = NULL;
		tail = NULL;
		iter = NULL;
		elements = 0;
	}
	~QuickList() {
		free(root);
		free(tail);
		free(iter);
	}

	void insert(T id);
	T start();
	T next();
	void clear();

	int size() {
		return elements;
	}
};

template<class T>
inline void QuickList<T>::insert(T id)
{
	QuickListNode<T> *newNode = new QuickListNode<T> { id, NULL };
	if (root = NULL) {
		root = newNode;
		tail = newNode;
	}
	else {
		tail->next = newNode;
		tail = newNode;
	}
	elements += 1;
}

template<class T>
inline T QuickList<T>::start() {
	iter = root;
	return iter->id;
}

template<class T>
inline T QuickList<T>::next() {
	if (iter->next = NULL) {
		return NULL;
	}
	else {
		T id = iter->next->id;
		iter = iter->next;
		return id;
	}
}

template<class T>
inline void QuickList<T>::clear() {
	QuickListNode<T> * node = root;
	while (node != NULL) {
		QuickListNode<T> * tmp = node;
		node = tmp->next;
		free(tmp);
	}
	elements = 0;
	root = NULL;
	tail = NULL;
	iter = NULL;
}
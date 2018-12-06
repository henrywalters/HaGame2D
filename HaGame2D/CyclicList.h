#pragma once

template <class T>
struct CyclicNode {
	CyclicNode<T> *parent;
	CyclicNode<T> *next;
	T value;
};

template <class T>
class CyclicList
{
	CyclicNode<T> *root;
	CyclicNode<T> *tail;
	int nodes;
public:
	CyclicList() {
		root = NULL;
		tail = NULL;
		nodes = 0;
	};

	~CyclicList() {
		free(root);
		free(tail);
	};

	void insert(T value);
	//void insertAfter(CyclicNode<T> * node, T value);
	//void remove(CyclicNode<T> * node);

	CyclicNode<T> * begin() { return root; }
	CyclicNode<T> * end() { return tail; }
	int size() { return nodes; }
};


template<class T>
void CyclicList<T>::insert(T value)
{
	CyclicNode<T> *node = new CyclicNode<T>;
	node->value = value;

	if (root == NULL) {
		tail = node;
		root = node;
		node->next = node;
	}
	else {
		tail->next = node;
		node->next = root;
		node->parent = tail;
		tail = node;
	}

	nodes += 1;
}

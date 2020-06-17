#pragma once
#include <functional>

namespace structures {

	template <class T>
	struct DoublyLinkedListNode {
		DoublyLinkedListNode<T>* prevNode;
		DoublyLinkedListNode<T>* nextNode;
		T value;
	};

	template <class T>
	class DoublyLinkedList {
		DoublyLinkedListNode<T>* _root;
		DoublyLinkedListNode<T>* _tail;
		int _count;

	public:

		DoublyLinkedList() {
			_root = NULL;
			_tail = NULL;
			_count = 0;
		}

		void addToBack(T value) {
			DoublyLinkedListNode<T> *node = new DoublyLinkedListNode<T>();
			node->value = value;
			node->prevNode = NULL;

			if (_root == NULL) {
				node->nextNode = NULL;

				_root = node;
				_tail = node;
			}
			else {
				_root->prevNode = node;
				node->nextNode = _root;
				_root = node;
			}

			_count++;
		}

		void addToFront(T value) {
			DoublyLinkedListNode<T>* node = new DoublyLinkedListNode<T>();
			node->value = value;
			node->nextNode = NULL;

			if (_tail == NULL) {
				node->prevNode = NULL;

				_root = node;
				_tail = node;
			}
			else {
				_tail->nextNode = node;
				node->prevNode = _tail;
				_tail = node;
			}

			_count++;
		}
		
		T removeFromFront() {
			if (_root != NULL) {
				DoublyLinkedListNode<T>* node = _root;
				if (node->nextNode) {
					node->nextNode->prevNode = NULL;
				}

				_root = node->nextNode;
				T value = node->value;
				//delete node;
				_count--;
				return value;
			}
		}

		T removeFromBack() {
			if (_tail != NULL) {
				DoublyLinkedListNode<T>* node = _tail;
				if (node->prevNode) {
					node->prevNode->nextNode = NULL;
				}
				_tail = node->prevNode;
				T value = node->value;
				//delete node;
				_count--;
				return value;
			}
		}

		T front() {
			return _root->value;
		}

		T back() {
			return _tail->value;
		}

		void iterate(std::function<void(T)> iterFunc) {
			DoublyLinkedListNode<T>* node = _root;

			while (node != NULL) {
				iterFunc(node->value);
				node = node->nextNode;
			}
		}

		void iterateBackwards(std::function<void(T)> iterFunc) {
			DoublyLinkedListNode<T>* node = _tail;

			while (node != NULL) {
				iterFunc(node->value);
				node = node->prevNode;
			}
		}
	};
}
#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <iostream>
#include <stdexcept>

using namespace std;
template <typename E>

class Node{
	public:
		E element;
		Node<E> * next;
		Node(E pElement, Node<E>* pNext = NULL){
		    element = pElement;
		    next = pNext;
		}
		Node(Node<E>* pNext = NULL){
		    next = pNext;
		}
};

#endif // NODE_H_INCLUDED

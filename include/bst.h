#ifndef __BINARY_SEARCH_TREE__H
#define __BINARY_SEARCH_TREE__H

#include <iostream>

template <typename T>
class BinarySearchTree
{
public:
	BinarySearchTree() {
        leftNode = nullptr;
        rightNode = nullptr;
        parent = nullptr;
        pData = nullptr;
	}
	~BinarySearchTree() {
        if(leftNode != nullptr) {
            delete leftNode;
        }
        if(rightNode != nullptr) {
            delete rightNode;
        }
        if(pData != nullptr) {
            delete pData;
        }
	}

	bool isEmpty() {
		return (pData == NULL);
	}

	void insertKey(T x) {
        if(pData == nullptr) {
            pData = new T();
            *pData = x;
        } else {
            if(pData -> get_movie_id() > x.get_movie_id()) {
                if(leftNode == nullptr) {
                    leftNode = new BinarySearchTree<T>;
                    leftNode -> parent = this;
                    leftNode -> pData = new T();
                    *(leftNode -> pData) = x;
                } else {
                    leftNode -> insertKey(x);
                }
            } else {
                if(rightNode == nullptr) {
                    rightNode = new BinarySearchTree<T>;
                    rightNode -> parent = this;
                    rightNode -> pData = new T();
                    *(rightNode -> pData) = x;
                } else {
                    rightNode -> insertKey(x);
                }
            }
        }
	}

	movie *searchKey(std::string x) {
        if(pData == nullptr) {
            return nullptr;
        } else {
            if(pData -> get_movie_id() == x) {
                return pData;
            } else {
                if(pData -> get_movie_id() > x) {
                    if(leftNode == nullptr) {
                        return nullptr;
                    } else {
                        return leftNode -> searchKey(x);
                    }
                } else {
                    if(rightNode == nullptr) {
                        return nullptr;
                    } else {
                    	return rightNode -> searchKey(x);
										}
                }
            }
        }
	}

	void inOrderDisplay() {
        if(leftNode != nullptr) {
            leftNode -> inOrderDisplay();
        }
        if(pData != nullptr) {
            std::cout << pData -> get_movie_id() << " ";
        }
        if(rightNode != nullptr) {
            rightNode -> inOrderDisplay();
        }
	}

private:
	BinarySearchTree<T> *leftNode;
	BinarySearchTree<T> *rightNode;
	BinarySearchTree<T> *parent;
	T *pData;
};

#endif // __BINARY_SEARCH_TREE_H

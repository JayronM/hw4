#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:

    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    AVLNode<Key,Value>* insertHelper(AVLNode<Key,Value>* node,AVLNode<Key,Value>* newnode);
    AVLNode<Key,Value>* rebalance(AVLNode<Key,Value>* node);
    AVLNode<Key,Value>* leftRotate(AVLNode<Key,Value>* node);
    AVLNode<Key,Value>* rightRotate(AVLNode<Key,Value>* node);
    int height(AVLNode<Key,Value>* node) const;
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */

 //help to count gheight
template<class Key, class Value>
int AVLTree<Key, Value>::height(AVLNode<Key, Value>* node) const
{
    if(node == nullptr){
			return 0;
		}

    return std::max(height(node->getLeft()), height(node->getRight())) +1;

}

 //rotate node with rright child, going toward the left, counterclockwise per se
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::leftRotate(AVLNode<Key, Value>* crt) 
{
    AVLNode<Key, Value>* y = crt->getRight();

    crt->setRight(y->getLeft());
    if(y->getLeft()!= nullptr){
        y->getLeft()->setParent(crt);
    }
    y->setParent(crt->getParent());

    if(crt->getParent()==nullptr){
        this->root_=y;
    }else if(crt==crt->getParent()->getLeft()){
        crt->getParent()->setLeft(y);
    }else{
        crt->getParent()->setRight(y);
    }
    y->setLeft(crt);
    crt->setParent(y);

    int leftHt = height(crt->getLeft());
    int rightHt = height(crt->getRight());
		crt->setBalance(rightHt-leftHt);

		leftHt = height(y->getLeft());
		rightHt = height(y->getRight());
    y->setBalance(rightHt-leftHt);
    
		return y;
}
//rotate node with left child, going toward the right, clockwise per se
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rightRotate(AVLNode<Key, Value>* y) 
{
    AVLNode<Key, Value>* rot = y->getLeft();
    y->setLeft(rot->getRight());
    if(rot->getRight()!= nullptr){
        rot->getRight()->setParent(y);
    }
    rot->setParent(y->getParent());
    if(y->getParent()==nullptr){
        this->root_=rot;
    }else if(y==y->getParent()->getLeft()){
        y->getParent()->setLeft(rot);
    }else{
        y->getParent()->setRight(rot);
    }
    rot->setRight(y);
    y->setParent(rot);

    int leftHt = height(y->getLeft());
    int rightHt = height(y->getRight());
    y->setBalance(rightHt-leftHt);
     
		leftHt = height(rot->getLeft());
		rightHt = height(rot->getRight());
		rot->setBalance(rightHt -leftHt);
		return rot;
}

//rebalnce if balance is off
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rebalance(AVLNode<Key, Value>* node)
{
    if(node->getBalance()==2){
        AVLNode<Key, Value>* rightHV = node->getRight(); //right heavy
        if(rightHV!= nullptr && rightHV->getBalance()< 0){
            node->setRight(rightRotate(rightHV));
        }
        return leftRotate(node);
    }
    if(node->getBalance()==-2){ //heavy on the left side
        AVLNode<Key, Value>* leftHV = node->getLeft(); //right heavy
        if(leftHV!= nullptr && leftHV->getBalance()> 0){
            node->setLeft(leftRotate(leftHV));
        }
        return rightRotate(node);
    }
    return node;
}


//insertation helper
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::insertHelper(AVLNode<Key, Value>* node, AVLNode<Key, Value>* newnode)
{
		if(node ==nullptr){
			return newnode;
		}
    if(newnode->getKey()<node->getKey()){
        AVLNode<Key, Value>* leftChd = insertHelper(node->getLeft(),newnode); 
        node->setLeft(leftChd);
				leftChd->setParent(node);
    }
    else if (newnode->getKey() > node->getKey()){
			AVLNode<Key, Value>* rightChd = insertHelper(node->getRight(),newnode); 
      node->setRight(rightChd);
			rightChd->setParent(node);
		} else{
			node->setValue(newnode->getValue());
			delete newnode;
			return node;
		}

		int leftHt = height(node->getLeft());
		int rightHt = height(node->getRight());
		node->setBalance(rightHt-leftHt);

		return rebalance(node);
}


template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
		if(this->root_ == nullptr){
			this->root_ = new AVLNode<Key,Value>(new_item.first, new_item.second, nullptr);
			return;
		}
		AVLNode<Key, Value>* newnode = new AVLNode<Key,Value>(new_item.first, new_item.second, nullptr);
		this->root_ = insertHelper(static_cast<AVLNode<Key, Value>*>(this->root_), newnode);

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
	AVLNode<Key, Value>* removedNode = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
	if(removedNode ==nullptr){
		return;
	}
	//two children swap with predecessor
	if(removedNode->getLeft()!= nullptr&& removedNode->getRight()!= nullptr){
		AVLNode<Key, Value>* predsr = static_cast<AVLNode<Key, Value>*>(this->predecessor(removedNode));
		this->nodeSwap(removedNode, predsr);
		removedNode = predsr;
	}

	//only having one child
	AVLNode<Key, Value>* child = nullptr;
	if(removedNode->getLeft()!= nullptr){
		child = static_cast<AVLNode<Key, Value>*>(removedNode->getLeft());
	} else{
		child = static_cast<AVLNode<Key, Value>*>(removedNode->getRight());
	}

	AVLNode<Key, Value>* parent = static_cast<AVLNode<Key, Value>*>(removedNode->getParent());
	if(child!= nullptr){
		child->setParent(parent);
	}
	if(parent ==nullptr){
		this->root_ = child;
		delete removedNode;
	} else{
		if(parent->getLeft() == removedNode){
			parent->setLeft(child);
		} else{
			parent->setRight(child);
		}
		delete removedNode;

		//rebalnce from parent
		AVLNode<Key, Value>* node = parent;
		while(node!=nullptr){
			int leftHt = height(node->getLeft());
			int rightHt = height(node->getRight());

			node->setBalance(rightHt-leftHt);

			if(node->getBalance() ==2 || node->getBalance()==-2){
				AVLNode<Key, Value>* subtree = rebalance(node);
				
				AVLNode<Key, Value>* nextParent = subtree->getParent();

				if(nextParent==nullptr){
					this->root_ =subtree;
				} else{
					if(node ==nextParent->getLeft()){
						nextParent->setLeft(subtree);
					} else{
						nextParent->setRight(subtree);
					}
				}
				node = node->getParent();
			}
		}
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif

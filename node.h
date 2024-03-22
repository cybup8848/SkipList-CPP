/*************************************************************************
    > File Name: node.h
    > Author:cheng yingbin
    > Mail: 703477993@qq.com 
    > Created Time: Thu Mar 21 21:56:47 2024
 ************************************************************************/
#ifndef _NODE_H_
#define _NODE_H_

#include"lib.h"
template<typename K,typename V>
class Node{
	public:
		Node(){}

		Node(K,V,int);
		
		~Node();

		K getKey() const;

		V getValue() const;
		void setValue(V);

		int getNodeLevel() const;
		void setNodeLevel(int);

		Node **_forward;
	private:
		K _key;
		V _value;
		int _nodeLevel;
};

template<typename K,typename V>
Node<K,V>::Node(K k,V v,int level){
	this->_key=k;
	this->_value=v;
	this->_nodeLevel=level;
	this->_forward=new Node<K,V>*[level+1];
	memset(this->_forward,0,sizeof(Node<K,V>*)*(level+1));
}

template<typename K,typename V>
Node<K,V>::~Node(){
	delete[] this->_forward;
	this->_forward=nullptr;
}


template<typename K,typename V>
K Node<K,V>::getKey() const{
	return this->_key;
}

template<typename K,typename V>
V Node<K,V>::getValue() const{
	return this->_value;
}

template<typename K,typename V>
void Node<K,V>::setValue(V v){
	this->_value=v;
}

template<typename K,typename V>
int Node<K,V>::getNodeLevel() const{
	return this->_nodeLevel;
}

template<typename K,typename V>
void Node<K,V>::setNodeLevel(int level){
	this->_nodeLevel=level;
}


#endif





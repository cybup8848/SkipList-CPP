/*************************************************************************
    > File Name: skiplist.h
    > Author:cheng yingbin
*************************************************************************
    > File Name: skiplist.h
    > Author:cheng yingbin
    > Mail: 703477993@qq.com 
    > Created Time: Thu Mar 21 22:10:01 2024
 ************************************************************************/
#ifndef _SKIPLIST_H_
#define _SKIPLIST_H_

#include"node.h"
//std::mutex mx;
const std::string DUMPFILE="store/dumpFile";
const std::string LOADFILE="store/loadFile";
const std::string delimiter=":";

template<typename K,typename V>
class SkipList{
	public:
		SkipList(int maxLevel=50);
		
		~SkipList();


		Node<K,V>* createNode(K,V,int);
		
		bool insertElement(K,V);

		void displayList() const;

		bool searchElement(K) const;

		void deleteElement(K);

		void dumpFile();

		void loadFile();
		
		int size() const;
		
		void clear();

	private:
		//递归删除节点
		void clear1(Node<K,V>*);

		//顺序删除节点
		void clear2();

		void getKeyValueFromString(const std::string& str,std::string &key,std::string &value);
		bool isValidString(const std::string& str);
		int getRandomLevel();
	private:
		int _maxLevel;

		int _skipListLevel;
		
		Node<K,V>* _header;

		std::ofstream _fileWriter;

		std::ifstream _fileReader;

		int _elementCount;

		std::mutex mx;
};


template<typename K,typename V>
SkipList<K,V>::SkipList(int maxLevel){
	this->_maxLevel=maxLevel;
	this->_skipListLevel=0;
	this->_elementCount=0;
	
	K k;
	V v;
	this->_header=new Node<K,V>(k,v,maxLevel);
}

template<typename K,typename V>
SkipList<K,V>::~SkipList(){
	if(this->_fileWriter.is_open()){
		this->_fileWriter.close();
	}
	if(this->_fileReader.is_open()){
		this->_fileReader.close();
	}
	this->_maxLevel=0;
	this->_skipListLevel=0;
	this->_elementCount=0;

	Node<K,V> *head=this->_header;
	Node<K,V> *pre=nullptr;
	while(head!=nullptr){
		pre=head;
		head=head->_forward[0];
		delete pre;
		pre=nullptr;
	}
	this->_header=nullptr;
}

template<typename K,typename V>
Node<K,V>* SkipList<K,V>::createNode(K k,V v,int level){
	return new Node<K,V>(k,v,level);
}

template<typename K,typename V>
bool SkipList<K,V>::insertElement(K key,V value){
	std::lock_guard<std::mutex> lock(mx);

	Node<K,V>** update=new Node<K,V>*[this->_maxLevel+1];
	memset(update,0,sizeof(Node<K,V>*)*(this->_maxLevel+1));
	Node<K,V> *current=this->_header;
	for(int i=this->_skipListLevel;i>=0;i--){
		while(current->_forward[i]!=nullptr&&current->_forward[i]->getKey()<key){
			current=current->_forward[i];
		}
		update[i]=current;
	}
	
	current=current->_forward[0];
	if(current!=nullptr&&current->getKey()==key){//已经存在
		std::cout<<"already exist"<<std::endl;
		delete[] update;
		update=nullptr;
		//mx.unlock();
		return false;
	}

	int level=this->getRandomLevel();
	for(int i=this->_skipListLevel+1;i<=level;i++){
		update[i]=this->_header;
	}
	this->_skipListLevel=std::max(level,this->_skipListLevel);

	Node<K,V> *insertNode=this->createNode(key,value,level);
	for(int i=0;i<=level;i++){
		insertNode->_forward[i]=update[i]->_forward[i];
		update[i]->_forward[i]=insertNode;
	}
	this->_elementCount++;
	std::cout<<"insert ("<<key<<","<<value<<")"<<" successfully"<<std::endl;
	delete[] update;
	update=nullptr;
	return true;
}

template<typename K,typename V>
void SkipList<K,V>::displayList() const{
	for(int i=this->_skipListLevel;i>=0;i--){
		Node<K,V> *current=this->_header->_forward[i];
		std::cout<<i<<": ";
		while(current!=nullptr){
			std::cout<<"("<<current->getKey()<<","<<current->getValue()<<") ";
			current=current->_forward[i];
		}
		std::cout<<std::endl;
	}
}

template<typename K,typename V>
bool SkipList<K,V>::searchElement(K key) const{
	std::cout<<"search element---------------"<<std::endl;
	
	Node<K,V> *current=this->_header;
	for(int i=this->_skipListLevel;i>=0;i--){
		while(current->_forward[i]!=nullptr&&current->_forward[i]->getKey()<key){
			current=current->_forward[i];
		}
	}
	current=current->_forward[0];
	if(current!=nullptr&&current->getKey()==key){
		std::cout<<"Found key:"<<key<<" value:"<<current->getValue()<<std::endl;
		return true;
	}
	std::cout<<"Sorry,not found"<<std::endl;
	return false;
}

template<typename K,typename V>
void SkipList<K,V>::deleteElement(K key){
	std::lock_guard<std::mutex> lock(mx);

	Node<K,V>** update=new Node<K,V>*[this->_skipListLevel+1];
	Node<K,V> *current=this->_header;
	for(int i=this->_skipListLevel;i>=0;i--){
		while(current->_forward[i]!=nullptr&&current->_forward[i]->getKey()<key){
			current=current->_forward[i];
		}
		update[i]=current;
	}
	current=current->_forward[0];
	if(current!=nullptr&&current->getKey()==key){
		std::cout<<"Found key:"<<key<<"  value:"<<current->getValue()<<std::endl;
		int level=current->getNodeLevel();
		for(int i=0;i<=level;i++){
			update[i]->_forward[i]=current->_forward[i];
		}
		this->_elementCount--;
	}
	delete[] update;
	update=nullptr;
}

template<typename K,typename V>
void SkipList<K,V>::dumpFile(){
	std::cout<<"dump file-----------------"<<std::endl;

	this->_fileWriter.open(DUMPFILE,std::ios_base::out);
	Node<K,V> *current=this->_header->_forward[0];
	while(current!=nullptr){
		this->_fileWriter<<current->getKey()<<":"<<current->getValue()<<std::endl;
		std::cout<<"("<<current->getKey()<<","<<current->getValue()<<") ";
		current=current->_forward[0];
	}
	std::cout<<std::endl;
	this->_fileWriter.flush();
	this->_fileWriter.close();
}

template<typename K,typename V>
void SkipList<K,V>::loadFile(){
	std::cout<<"load file----------------"<<std::endl;
	
	this->_fileReader.open(LOADFILE);
	
	std::string line="";
	std::string key="";
	std::string value="";
	while(getline(this->_fileReader,line)){
		if(!isValidString(line)){
			continue;
		}
		this->getKeyValueFromString(line,key,value);
		this->insertElement(std::stoi(key),value);
		std::cout<<"key:"<<key<<" value:"<<value<<std::endl;
	}
	this->_fileReader.close();
}

template<typename K,typename V>
void SkipList<K,V>::getKeyValueFromString(const std::string& str,std::string &key,std::string &value){
	int pos=str.find(delimiter);
	key=str.substr(0,pos);
	value=str.substr(pos+1);
}

template<typename K,typename V>
bool SkipList<K,V>::isValidString(const std::string &str){
	if(str.empty()){
		return false;
	}
	if(str.find(delimiter)==std::string::npos){
		return false;
	}
	return true;
}

template<typename K,typename V>
int SkipList<K,V>::getRandomLevel(){
	/*
	std::default_random_engine e;
	std::uniform_int_distribution<int> u(0,this->_maxLevel);
	e.seed((unsigned int)time(nullptr));
	return u(e);
	*/

	std::random_device rd;//生成随机数种子
	std::mt19937 gen(rd());//定义随机数生成引擎
	std::uniform_int_distribution<int> uint(0,this->_maxLevel);
	return uint(gen);
}

template<typename K,typename V>
int SkipList<K,V>::size() const{
	return this->_elementCount;
}

template<typename K,typename V>
void SkipList<K,V>::clear(){
	Node<K,V> *current=this->_header->_forward[0];
	Node<K,V> *pre=nullptr;
	while(current!=nullptr){
		pre=current;
		current=current->_forward[0];
		delete pre;
		pre=nullptr;
	}
	
	this->_elementCount=0;
	this->_skipListLevel=0;
	for(int i=0;i<this->_maxLevel;i++){
		this->_header->_forward[i]=nullptr;
	}
	if(this->_fileWriter.is_open()){
		this->_fileWriter.close();
	}
	if(this->_fileReader.is_open()){
		this->_fileReader.close();
	}
}


template<typename K,typename V>
void SkipList<K,V>::clear1(Node<K,V> *header){
	if(header==nullptr){
		return;
	}
	clear1(header->_forward[0]);
	delete header;
}

template<typename K,typename V>
void SkipList<K,V>::clear2(){
	Node<K,V> *pre=nullptr;
	Node<K,V> *head=this->_header;
	while(head!=nullptr){
		pre=head;
		head=head->_forward[0];
		delete pre;
		pre=nullptr;
	}
	this->_header=nullptr;
}

#endif


/*************************************************************************
    > File Name: main.cpp
    > Author:cheng yingbin
    > Mail: 703477993@qq.com 
    > Created Time: Fri Mar 22 11:11:00 2024
 ************************************************************************/
#include"skiplist.h"
int main(int argc,char *argv[]){
	SkipList<int,std::string> skipList;

	skipList.insertElement(1,"今天");
	skipList.insertElement(2,"写完");
	skipList.insertElement(5,"跳表");
	skipList.insertElement(8,"就去");
	skipList.insertElement(10,"东方艺术中心");
	skipList.insertElement(14,"汉诺塔");
	skipList.insertElement(16,"听");
	skipList.insertElement(26,"贝多芬");
	skipList.insertElement(38,"命运交响曲");
	

	std::cout<<"skiplist size:"<<skipList.size()<<std::endl;


	skipList.dumpFile();
	std::cout<<"\n\n\n"<<std::endl;


	skipList.loadFile();
	
	skipList.searchElement(5);
	skipList.searchElement(10);
	skipList.searchElement(200);
	std::cout<<"\n\n\n"<<std::endl;

	skipList.displayList();
	std::cout<<"\n\n\n"<<std::endl;

	skipList.deleteElement(3);
	skipList.deleteElement(14);
	std::cout<<"\n\n\n"<<std::endl;

	
	std::cout<<"skiplist size:"<<skipList.size()<<std::endl;
	std::cout<<"\n\n\n"<<std::endl;
		
	skipList.displayList();
	std::cout<<"\n\n\n"<<std::endl;


	return 0;
}












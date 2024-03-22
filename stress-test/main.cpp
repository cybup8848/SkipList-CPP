/*************************************************************************
    > File Name: main.cpp
    > Author:cheng yingbin
    > Mail: 703477993@qq.com 
    > Created Time: Fri Mar 22 16:19:09 2024
 ************************************************************************/

#include"../skiplist.h"
#include<thread>
#include<chrono>
#include<functional>
const int TEST_COUNT=100000;
SkipList<int,std::string> skipList(20);

void insertElement(int num){
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> uint(1,TEST_COUNT);
	num=TEST_COUNT/num;
	for(int i=0;i<num;i++){
		int tmp=uint(gen);
		std::cout<<tmp<<std::endl;
		skipList.insertElement(tmp,"a");
	}
}

void getElement(int num){
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> uint(1,TEST_COUNT);
	num=TEST_COUNT/num;
	for(int i=0;i<num;i++){
		skipList.searchElement(uint(gen));
	}
}

void stressTest(std::function<void(int)> f){
	std::thread* threadArray[3];
    int threadNum[3]={1,5,10};

    for(int i=0;i<3;i++){
        int cn=threadNum[i];
        threadArray[i]=new std::thread[cn];

        auto start = std::chrono::high_resolution_clock::now();
        for(int j=0;j<cn;j++){
            threadArray[i][j]=std::thread(f,cn);
        }
        for(int j=0;j<cn;j++){
            threadArray[i][j].join();
        }
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        std::cout<<"test count:"<<TEST_COUNT<<"   thread number:"<<cn<<"        time:"<<elapsed.count()<<" seconds"<<std::endl;
        delete[] threadArray[i];
        threadArray[i]=nullptr;
		skipList.clear();
    }	
}

int main(int argc,char *argv[]){
	std::cout<<"insert element test:"<<std::endl;
	//stressTest(insertElement);//多线程产生了数据竞争，不知道如何解决
	std::cout<<"\n\n\n"<<std::endl;

	skipList.clear();
	
    auto start = std::chrono::high_resolution_clock::now();
	insertElement(1);
	auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout<<"test count:"<<TEST_COUNT<<"   thread number:1"<<"        time:"<<elapsed.count()<<" seconds"<<std::endl;

	std::cout<<"search element test:"<<std::endl;
	stressTest(getElement);
	std::cout<<"\n\n\n"<<std::endl;

	start=std::chrono::high_resolution_clock::now();
	getElement(1);
	finish=std::chrono::high_resolution_clock::now();
	std::cout<<"test count:"<<TEST_COUNT<<"	  thread number:1"<<"	time:"<<elapsed.count()<<" seconds"<<std::endl;

	
	return 0;
}
















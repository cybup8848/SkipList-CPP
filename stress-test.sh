#########################################################################
# File Name: stress-test.sh
# Author: cheng yingbin
# mail: 703477993@qq.com
# Created Time: Fri Mar 22 17:40:04 2024
#########################################################################
#!/bin/bash
g++ ./stress-test/main.cpp -o ./bin/stress --std=c++11 -pthread
./bin/stress



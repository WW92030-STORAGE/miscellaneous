#!/bin/bash
    
g++ main.cpp -o main -O0 -pg
./main
gprof ./main gmon.out > analysis.txt
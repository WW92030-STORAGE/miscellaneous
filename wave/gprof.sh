#!/bin/bash
    
g++ main.cpp -o main -Os -pg
./main
gprof ./main gmon.out > analysis.txt
rm main
rm gmon.out
#!/bin/bash
	
g++ main.cpp -o main -O3
./main > poissondisk.out
rm main

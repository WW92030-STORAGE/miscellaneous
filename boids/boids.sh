#!/bin/bash
	
g++ main.cpp -o main -O3
./main > boids.out
rm main

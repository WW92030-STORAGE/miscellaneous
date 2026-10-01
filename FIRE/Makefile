all:
	g++ main.cpp -o main -O3
	./main

build:
	g++ main.cpp -o main -O3
	
run:
	./main

render: build
	./main > FIRE.out
	python3 render.py

video: build
	./main
	python3 video.py -r 2
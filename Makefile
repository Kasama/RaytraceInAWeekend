all: compile run

compile: main.cpp
	gcc -lstdc++ -lm -std=c++17 -o main main.cpp

run: main
	./main > image.ppm

TARGET = so
FLAGS = -Wall -std=c++20 -lm
FILES = 

all:
	g++ $(FLAGS) $(FILES) main.cpp -o $(TARGET)
clear:
	rm -f $(TARGET)

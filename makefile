TARGET = so
FLAGS = -Wall -std=c++20 -lm
MOD_FOLDER = ./src/modules
FILES = $(wildcard $(MOD_FOLDER)/*.cpp)

all:
	g++ $(FLAGS) $(FILES) ./src/main.cpp -o $(TARGET)
run:
	./$(TARGET)
debug:
	./$(TARGET) -d
clear:
	rm -f $(TARGET)

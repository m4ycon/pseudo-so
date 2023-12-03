TARGET = main
FLAGS = -Wall -std=c++20 -lm
MOD_FOLDER = ./src/modules
FILES = $(wildcard $(MOD_FOLDER)/**/*.cpp) $(wildcard $(MOD_FOLDER)/*.cpp)

all:
	g++ $(FLAGS) $(FILES) ./src/main.cpp -o $(TARGET)
run:
	./$(TARGET) ./input/processes.txt ./input/files.txt

# Debug
main.exe: $(FILES) ./src/main.cpp
	g++ $(FLAGS) $(FILES) -g ./src/main.cpp -o $(TARGET)

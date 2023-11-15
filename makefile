TARGET = so
FLAGS = -Wall -std=c++20 -lm
MOD_FOLDER = ./src/modules
FILES = $(MOD_FOLDER)/memoryManager.cpp $(MOD_FOLDER)/scheduler.cpp $(MOD_FOLDER)/resourceManager.cpp $(MOD_FOLDER)/fileManager.cpp $(MOD_FOLDER)/process.cpp 

all:
	g++ $(FLAGS) $(FILES) ./src/main.cpp -o $(TARGET)
run:
	./$(TARGET)
debug: # show DCOUT prints
	./$(TARGET) -d
clear:
	rm -f $(TARGET)

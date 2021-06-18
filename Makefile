TARGET = webserv
CC = clang++
FLAG = -std=c++98
HEADER = -I includes
SRC = main.cpp ./src/*.cpp ./lib/*.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(FLAG) -o $(TARGET) $(SRC) $(HEADER) -fsanitize=address -g3

clean:
	rm -rf $(TARGET)
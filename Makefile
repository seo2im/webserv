TARGET = webserv
CC = clang++
FLAG = -Werror -Wall -Wextra -std=c++98
HEADER = -I includes
SRC = main.cpp \
	./src/app.cpp \
	./src/cgi.cpp \
	./src/config.cpp \
	./src/location.cpp \
	./src/request.cpp \
	./src/response.cpp \
	./src/server.cpp \
	./lib/ft_checkfile.cpp \
	./lib/ft_error.cpp \
	./lib/ft_gettime.cpp \
	./lib/ft_is_end_string.cpp \
	./lib/ft_nextline.cpp \
	./lib/ft_num2string.cpp \
	./lib/ft_split.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(FLAG) -o $(TARGET) $(OBJ) $(HEADER)

$(OBJ): %.o : %.cpp
	$(CC) $(FLAG) -c $< -o $@ $(HEADER)

clean:
	rm -rf $(OBJ)

fclean:
	rm -rf $(TARGET)

re: fclean all
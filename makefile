CC = gcc
XX = g++
TARGET = MyServer
LINK = $(XX)
PROGRAM_DIR = .

INCLUDE = -I$(PROGRAM_DIR)/include -I$(PROGRAM_DIR)/base


SRC = $(wildcard $(PROGRAM_DIR)/src/*.cpp)
BASE_SRC = $(wildcard $(PROGRAM_DIR)/base/*.cpp)
OBJ = $(patsubst %.cpp,%.o,$(SRC))
BASE_OBJ = $(patsubst %.cpp,%.o,$(BASE_SRC))

CFLAGS = -Wall -g $(INCLUDE) -std=c++11
CPPFLAGS = $(CFLAGS)
RM = rm

$(TARGET): $(OBJ) $(BASE_OBJ)
	@echo "Begin to build..."
	$(LINK) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(XX) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf src/*.o MyServer base/*.o
# Specify compiler
CC = g++

# Specify compiler flag
CCF = -std=c++17 -Wall -Wextra -g

# Specify include folder
INC = -Iinclude -I./

# Specify libs to use
LIB = -lcurlpp -lcurl

# Specify source file
SRC = example.cpp # src/include/*.cpp

# Specify out file
OUT = example

all: $(SRC)
	$(CC) $(CCF) $(INC) $(LIB) $(SRC) -o $(OUT)

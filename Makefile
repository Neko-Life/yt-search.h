# Specify compiler
CXX = g++

# Specify compiler flag
CCF = -std=c++17 -Wall -Wextra -g

# Specify include folder
INC = -Iinclude

# Specify libs to use
LIB = -lcurlpp -lcurl

# Specify source file
SRC = src/example.cpp src/yt-search/*.cpp # src/include/*.cpp

# Specify out file
OUT = example.out

all: $(SRC)
	$(CXX) $(CCF) $(INC) $(LIB) $(SRC) -o $(OUT)

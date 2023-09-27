# Specify compiler
CXX = g++

# Specify compiler flag
CCF = -std=c++17 -MMD -Wall -Wextra -g

# Specify include folder
INC = -Iinclude # -I../../include -I../curlpp/include -L../curlpp/build

# Specify libs to use
LIB = -lcurlpp -lcurl

# Specify source file
SRC = src/example.cpp $(wildcard src/yt-search/*.cpp) # src/include/*.cpp

OBJS = $(SRC:.cpp=.o)
DSFILES = $(SRC:.cpp=.d)
include $(DSFILES)

# Specify out file
OUT = example.out

all: $(OBJS)
	$(CXX) $(CCF) $(INC) $(LIB) $(OBJS) -o $(OUT)

$(DSFILES): $(SRC)
	$(CXX) $(CCF) $(INC) $(LIB) -c $(@:.d=.cpp) -o $(@:.d=.o)

clean:
	rm $(OBJS) $(DSFILES) $(OUT)

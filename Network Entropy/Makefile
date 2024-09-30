##
# Project Title
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Executable names
SERVER_EXEC = server
CLIENT_EXEC = client

# Source files
SERVER_SRC = NewServer.cpp
CLIENT_SRC = Client.cpp

# Object files
SERVER_OBJ = $(SERVER_SRC:.cpp=.o)
CLIENT_OBJ = $(CLIENT_SRC:.cpp=.o)

# Targets and dependencies
all: $(SERVER_EXEC) $(CLIENT_EXEC)

$(SERVER_EXEC): $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(CLIENT_EXEC): $(CLIENT_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -g -c $< -o $@

clean:
	rm -f $(SERVER_EXEC) $(CLIENT_EXEC) $(SERVER_OBJ) $(CLIENT_OBJ)

.PHONY: all clean

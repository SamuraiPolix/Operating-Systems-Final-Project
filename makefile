CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wunknown-pragmas -g

SRCS = MSTFactory.cpp Graph.cpp MSTSolver.cpp

MAIN = Server.cpp

OBJS = $(SRCS:.cpp=.o)

TARGET = server

all: $(TARGET)

.PHONY: all clean

$(TARGET): $(OBJS) $(MAIN)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: Test.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

Server.o: Server.cpp
	$(CXX) $(CXXFLAGS) -c $<

MSTFactory.o: MSTFactory.cpp MSTFactory.hpp
	$(CXX) $(CXXFLAGS) -c $<

MSTSolver.o: MSTSolver.cpp MSTSolver.hpp
	$(CXX) $(CXXFLAGS) -c $<

Graph.o: Graph.cpp Graph.hpp
	$(CXX) $(CXXFLAGS) -c $<

valgrind: $(TARGET)
	# valgrind --vgdb-error=0 --tool=memcheck -v --leak-check=full --show-leak-kinds=all  --error-exitcode=99 --track-origins=yes ./$(TARGET)
	valgrind --tool=helgrind ./$(TARGET)

clean:
	rm -f *.o $(TARGET) test
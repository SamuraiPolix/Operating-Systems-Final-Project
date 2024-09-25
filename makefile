CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wunknown-pragmas -g

SRCS = MSTFactory.cpp Graph.cpp MSTSolver.cpp

THREAD_POOL = ThreadPool.cpp ThreadPoolServer.cpp

MAIN = Server.cpp

OBJS = $(SRCS:.cpp=.o)

OBJS_THREADPOOL = $(THREAD_POOL:.cpp=.o)

TARGET = server

all: $(TARGET) threadpoll_server test

.PHONY: all clean

$(TARGET): $(OBJS) $(MAIN)
	$(CXX) $(CXXFLAGS) -o $@ $^

threadpoll_server: $(OBJS_THREADPOOL) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: Test.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

Server.o: Server.cpp
	$(CXX) $(CXXFLAGS) -c $<

ThreadPoolServer.o: ThreadPoolServer.cpp
	$(CXX) $(CXXFLAGS) -c $<

ThreadPool.o: ThreadPool.cpp ThreadPool.hpp
	$(CXX) $(CXXFLAGS) -c $<

MSTFactory.o: MSTFactory.cpp MSTFactory.hpp
	$(CXX) $(CXXFLAGS) -c $<

MSTSolver.o: MSTSolver.cpp MSTSolver.hpp
	$(CXX) $(CXXFLAGS) -c $<

Graph.o: Graph.cpp Graph.hpp
	$(CXX) $(CXXFLAGS) -c $<

valgrind_memcheck: $(TARGET)
	# valgrind --vgdb-error=0 --tool=memcheck -v --leak-check=full --show-leak-kinds=all  --error-exitcode=99 --track-origins=yes ./$(TARGET)
	valgrind --tool=helgrind ./$(TARGET)

valgrind_helgrind: $(TARGET)
	valgrind --tool=helgrind ./$(TARGET)

valgrind_cachegrind: $(TARGET)
	valgrind --tool=cachegrind ./$(TARGET)

clean:
	rm -f *.o $(TARGET) test threadpoll_server
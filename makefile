CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wunknown-pragmas -g

SRCS = MSTFactory.cpp Graph.cpp MSTSolver.cpp

THREAD_POOL = ThreadPool.cpp ThreadPoolServer.cpp

MAIN = Server.cpp

OBJS = $(SRCS:.cpp=.o)

OBJS_THREADPOOL = $(THREAD_POOL:.cpp=.o)

TARGET = server

# Dirs
OUTPUTS_FOLDER = outputs
INPUTS_FOLDER = inputs
EXPECTED_FOLDER = expected
GCOV_OUTPUTS = gcov_outputs
VALGRIND_OUTPUTS = valgrind_outputs
CALLGRIND_OUTPUTS = callgrind_outputs

all: $(TARGET) threadpoll_server test

.PHONY: all clean

# --------------------------------- Targets ---------------------------------
$(TARGET): $(OBJS) $(MAIN)
	$(CXX) $(CXXFLAGS) -o $@ $^

threadpoll_server: $(OBJS_THREADPOOL) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: Test.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# --------------------------------- Objects ---------------------------------
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

# --------------------------------- Code Coverage ---------------------------------
coverage: CXXFLAGS += --coverage
coverage: clean all
	./threadpoll_server
	./server
	./test
	gcov $(SRCS) $(MAIN) $(THREAD_POOL) > stdout1.txt || true
	gcov server-Server test-Test > stdout1.txt || true
	mv *.gcov $(GCOV_OUTPUTS)
	mv *.gcda $(GCOV_OUTPUTS)
	mv *.gcno $(GCOV_OUTPUTS)

# --------------------------------- GPROF ---------------------------------
profiling: Profiling.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

gprof: CXXFLAGS += -pg
gprof: profiling
	./$<
	gprof $< gmon.out > stdout1.txt

callgrind: CXXFLAGS += -g -O0
callgrind: profiling
	valgrind --tool=callgrind --callgrind-out-file=$(CALLGRIND_OUTPUTS)/callgrind.out ./$< 2> $(CALLGRIND_OUTPUTS)/callgrind_stdout.txt
	callgrind_annotate $(CALLGRIND_OUTPUTS)/callgrind.out > $(CALLGRIND_OUTPUTS)/callgrind_annotate.txt
	kcachegrind $(CALLGRIND_OUTPUTS)/callgrind

# ----------------------------------- Valgrind -----------------------------------
valgrind_memcheck: $(TARGET) threadpoll_server
	valgrind --tool=memcheck -v --leak-check=full --show-leak-kinds=all  --error-exitcode=99 --track-origins=yes ./$(TARGET) 2> $(VALGRIND_OUTPUTS)/server_stdout.txt
	valgrind --tool=memcheck -v --leak-check=full --show-leak-kinds=all  --error-exitcode=99 --track-origins=yes ./threadpoll_server 2> $(VALGRIND_OUTPUTS)/threadpoll_server_stdout.txt

valgrind_helgrind: $(TARGET) threadpoll_server
	valgrind --tool=helgrind ./$(TARGET)
	valgrind --tool=helgrind ./threadpoll_server

valgrind_cachegrind: $(TARGET) threadpoll_server
	valgrind --tool=cachegrind ./$(TARGET)
	valgrind --tool=cachegrind ./threadpoll_server

# ----------------------------------- tidy -----------------------------------
tidy:
	clang-tidy $(SRCS) $(MAIN) $(THREAD_POOL) -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory --warnings-as-errors=-* --
	# clang-tidy $(SRCS) -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory --warnings-as-errors=-* --


clean:
	rm -f *.o $(TARGET) test threadpoll_server profiling
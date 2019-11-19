CXX=g++
CXXFLAGS= -std=c++11 -g -fprofile-arcs

LINKFLAGS=

SRC_DIR = src

INCLUDE = include

PROGRAM = hTest

.PHONY: all
all: clean-all $(PROGRAM)

# default rule for compiling .cc to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf *~ $(SRC)/*.o *.gcov *.gcda *.gcno *\#*


.PHONY: clean-all
clean-all: clean
	rm -rf $(PROGRAM)

$(PROGRAM):
	$(CXX) $(CXXFLAGS) -o $(PROGRAM) -I $(INCLUDE) $(SRC_DIR)/*.cpp $(LINKFLAGS)


memcheck: $(PROGRAM)
	valgrind --tool=memcheck --leak-check=yes $(PROGRAM)

static: ${SRC_DIR}
	cppcheck --verbose --enable=all --xml ${SRC_DIR} ${INCLUDE} --suppress=missingInclude

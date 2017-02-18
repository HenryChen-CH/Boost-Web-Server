BOOSTFLAGS = -l boost_system -lboost_log -pthread -std=c++0x -g -Wall -DBOOST_LOG_DYN_LINK
TEST_FLAGS =-l boost_system -lboost_log -pthread -std=c++0x -g -Wall -fprofile-arcs -ftest-coverage -DBOOST_LOG_DYN_LIN

CPP_FILES = $(wildcard ./*.cc)
OBJ_FILES := $(addprefix ,$(notdir $(CPP_FILES:.cpp=.o)))

main: $(OBJ_FILES)
	g++ -std=c++11 $(LD_FLAGS) -o $@ $^
.PHONY: main

%.o: %.cpp
	g++ -std=c++11 $(CC_FLAGS) -c -o $@ $<

clean:
	@rm -rf obj main *.o *.d

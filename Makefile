BOOST_FLAGS =-lboost_system -lboost_log -pthread -g -Wall -DBOOST_LOG_DYN_LINK
TEST_FLAGS =-lboost_system -lboost_log -pthread -g -Wall -fprofile-arcs -ftest-coverage -DBOOST_LOG_DYN_LIN

GTEST_DIR = googletest/googletest
CPP_FILES = $(wildcard ./*.cc)
OBJ_FILES := $(addprefix ,$(notdir $(CPP_FILES:.cpp=.o)))
NGINX_FILES=nginx-configparser/config_parser.cc

main: $(OBJ_FILES) $(NGINX_FILES)
	g++ -isystem ${GTEST_DIR}/include -std=c++0x $^ $(BOOST_FLAGS) -o $@
.PHONY: main

%.o: %.cpp
	g++ -std=c++11 $(BOOST_FLAGS) -isystem ${GTEST_DIR}/include -c -o $@ $<

clean:
	@rm -rf obj main *.o *.d

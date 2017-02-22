BOOST_FLAGS =-lboost_system -lboost_log -pthread -g -Wall -DBOOST_LOG_DYN_LINK
TEST_FLAGS =-lboost_system -lboost_log -pthread -g -Wall -fprofile-arcs -ftest-coverage -DBOOST_LOG_DYN_LIN

GTEST_DIR = googletest/googletest
CPP_FILES = $(wildcard ./src/*.cc)
OBJ_FILES := $(notdir $(CPP_FILES:.cc=.o))
NGINX_FILES=nginx-configparser/config_parser.cc

webserver: $(OBJ_FILES) $(NGINX_FILES)
	g++ -isystem ${GTEST_DIR}/include -std=c++0x $^ $(BOOST_FLAGS) -o $@
.PHONY: webserver

%.o: src/%.cc $(NGINX_FILES)
	g++ -std=c++11 $(BOOST_FLAGS) -isystem ${GTEST_DIR}/include -I . -c -o $@ $<

clean:
	@rm -rf obj webserver *.o *.d

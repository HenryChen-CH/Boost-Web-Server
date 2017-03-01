BOOST_FLAGS = -lboost_system -lboost_log_setup -lboost_log -DBOOST_LOG_DYN_LINK
TEST_FLAGS = -fprofile-arcs -ftest-coverage
COMMON_FLAGS = -std=c++11 -pthread -g -Wall

GTEST_DIR = googletest/googletest
CPP_FILES = $(wildcard ./src/*.cc)
OBJ_FILES := $(notdir $(CPP_FILES:.cc=.o))
NGINX_FILES= nginx-configparser/config_parser.cc
TEST_FILES = $(wildcard ./test/*_test.cc)
TESTS = $(TEST_FILES:%.cc=%)
CPP_EXCEPT_MAIN = $(filter-out ./src/webserver_main.cc, $(CPP_FILES))

webserver: $(OBJ_FILES) $(NGINX_FILES)
	g++ $(COMMON_FLAGS) $^ $(BOOST_FLAGS) -o $@
.PHONY: webserver

%.o: src/%.cc $(NGINX_FILES)
	g++ -std=c++11 $(BOOST_FLAGS) -I ${GTEST_DIR}/include -I . -c -o $@ $<

test/%_test : test/%_test.cc libgtest.a src/%.cc
	g++ -isystem ${GTEST_DIR}/include -I ./src -I . $(CPP_EXCEPT_MAIN) $(NGINX_FILES) ${GTEST_DIR}/src/gtest_main.cc $(word 1, $^) $(word 2, $^) $(TEST_FLAGS) $(COMMON_FLAGS) $(BOOST_FLAGS) -o $@ 
	$@
	rm $@
	@mv *.gcno src/
	@mv *.gcda src/
	gcov -r $(word 3, $^)
	
test: $(TESTS) webserver
	chmod a+x integration-test.py
	./integration-test.py
	./proxy-test.sh
.PHONY: test

proxy:
	./proxy-test.sh

libgtest.a: $(GTEST_DIR)
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

clean:
	@rm -rf obj webserver *.o *.d *.a src/*.gcno src/*.gcda *.gcov *.gcno *.gcda

print-%  : ; @echo $* = $($*)

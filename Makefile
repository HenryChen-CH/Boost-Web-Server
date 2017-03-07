BOOST_FLAGS = -lboost_log_setup -lboost_log -lboost_thread -lboost_system -DBOOST_LOG_DYN_LINK
TEST_FLAGS = -fprofile-arcs -ftest-coverage
COMMON_FLAGS = -std=c++11 -pthread -g -Wall

GTEST_DIR = googletest/googletest
CPP_FILES = $(wildcard ./src/*.cc)
OBJ_FILES := $(notdir $(CPP_FILES:.cc=.o))
NGINX_FILE= nginx-configparser/config_parser.cc
NGINX_OBJ= nginx-configparser/config_parser.o
TEST_FILES = $(wildcard ./test/*_test.cc)
TESTS = $(TEST_FILES:%.cc=%)
CPP_EXCEPT_MAIN = $(filter-out ./src/webserver_main.cc, $(CPP_FILES))

webserver: $(OBJ_FILES) $(NGINX_OBJ)
	g++ -static-libgcc -static-libstdc++ $(COMMON_FLAGS)  $^ $(BOOST_FLAGS) -Wl,-Bstatic -o $@
.PHONY: webserver

nginx-configparser/config_parser.o: $(NGINX_FILE)
	g++ -std=c++11 -g $(BOOST_FLAGS) -I ${GTEST_DIR}/include -I . -c -o $@ $<

%.o: src/%.cc
	g++ -std=c++11 -g $(BOOST_FLAGS) -I ${GTEST_DIR}/include -I . -c -o $@ $<

test/%_test : test/%_test.cc libgtest.a src/%.cc
	g++ -isystem ${GTEST_DIR}/include -I ./src -I . $(CPP_EXCEPT_MAIN) $(NGINX_FILE) ${GTEST_DIR}/src/gtest_main.cc $(word 1, $^) $(word 2, $^) $(TEST_FLAGS) $(COMMON_FLAGS) $(BOOST_FLAGS) -o $@ 
	$@
	rm $@
	@mv *.gcno src/
	@mv *.gcda src/
	gcov -r $(word 3, $^)
	
test: $(TESTS) webserver
	chmod a+x integration-test.py
	./integration-test.py
	chmod a+x multi_thread_integration_test.py
	./multi-thread-integration-test.py
	./proxy-test.sh
.PHONY: test

proxy:
	./proxy-test.sh

libgtest.a: $(GTEST_DIR)
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

clean:
	@rm -rf obj webserver *.o *.d *.a src/*.gcno src/*.gcda *.gcov *.gcno *.gcda

docker-build:
	docker build -t httpserver .

docker-run: docker-build
	docker run --rm -t -p 3000:3000 httpserver

print-%  : ; @echo $* = $($*)

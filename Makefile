FLAGS = -l boost_system -pthread -std=c++0x -g -Wall
TEST_FLAGS =-l boost_system -pthread -std=c++0x -g -Wall -fprofile-arcs -ftest-coverage
SOURCE_FILES = http_server.cpp nginx-configparser/config_parser.cc webserver_main.cc connection.cpp connection_manager.cpp request_handler.cpp request_parser.cpp response.cpp request.cpp
TEST_FILES = http_server.cpp nginx-configparser/config_parser.cc connection.cpp connection_manager.cpp request_handler.cpp request_parser.cpp response.cpp request.cpp
GTEST_DIR = googletest/googletest

all: compile

compile: $(SOURCE_FILES)
	@g++ $(SOURCE_FILES) $(FLAGS) -o webserver

clean1:
	@rm -rf webserver *.dSYM *.o *.a *.gcda *.gcno *.gcov

integration-test: compile
	chmod a+x integration-test.py
	./integration-test.py

http_server_test: gtest
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc http_server_test.cpp libgtest.a $(TEST_FLAGS) -o http_server_test
	./http_server_test
.PHONY: http_server_test

request_handler_test: gtest
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc request_handler_test.cpp libgtest.a $(TEST_FLAGS) -o request_handler_test
	./request_handler_test
.PHONY: request_handler_test

request_parser_test: gtest
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc request_parser_test.cpp libgtest.a $(TEST_FLAGS) -o request_parser_test
	./request_parser_test
.PHONY: request_parser_test

request_test: gtest
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc request_test.cpp libgtest.a $(TEST_FLAGS) -o request_test
	./request_test
.PHONY: request_test

response_test: gtest
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc response_test.cpp libgtest.a $(TEST_FLAGS) -o response_test
	./response_test
.PHONY: response_test

connection_test: gtest
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc connection_test.cpp libgtest.a $(TEST_FLAGS) -o connection_test
	./connection_test
.PHONY: connection_test

connection_manager_test: gtest
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc connection_manager_test.cpp libgtest.a $(TEST_FLAGS) -o connection_manager_test
	./connection_manager_test
.PHONY: connection_manager_test

gtest: $(GTEST_DIR)
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

unittest: http_server_test request_handler_test request_parser_test request_test response_test connection_test connection_manager_test

clean: clean1
	rm -rf http_server_test request_handler_test request_parser_test request_test response_test connection_test connection_manager_test

test: unittest integration-test

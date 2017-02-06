FLAGS = -l boost_system -pthread -std=c++0x -g -Wall
TEST_FLAGS =-l boost_system -pthread -std=c++0x -g -Wall -fprofile-arcs -ftest-coverage
SOURCE_FILES = http_server.cpp nginx-configparser/config_parser.cc webserver_main.cc connection.cpp \
connection_manager.cpp request_parser.cpp response.cpp request.cpp request_handler_echo.cpp \
request_handler_file.cpp mime_types.cpp
TEST_FILES = http_server.cpp nginx-configparser/config_parser.cc connection.cpp connection_manager.cpp \
 request_parser.cpp response.cpp request.cpp mime_types.cpp request_handler_echo.cpp request_handler_file.cpp
GTEST_DIR = googletest/googletest

all: compile

compile: $(SOURCE_FILES)  ${GTEST_DIR}/include
	g++ -isystem  ${GTEST_DIR}/include $(SOURCE_FILES) $(FLAGS) -o webserver

clean1:
	@rm -rf webserver *.dSYM *.o *.a *.gcda *.gcno *.gcov

integration-test: compile
	chmod a+x integration-test.py
	./integration-test.py

http_server_test: libgtest.a
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc http_server_test.cpp libgtest.a $(TEST_FLAGS) -o http_server_test
	./http_server_test
	gcov -r http_server.cpp
.PHONY: http_server_test

request_handler_echo_test: libgtest.a
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc request_handler_echo_test.cpp libgtest.a $(TEST_FLAGS) -o request_handler_echo_test
	./request_handler_echo_test
	gcov -r request_handler_echo.cpp
.PHONY: request_handler_echo_test

request_parser_test: libgtest.a
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc request_parser_test.cpp libgtest.a $(TEST_FLAGS) -o request_parser_test
	./request_parser_test
	gcov -r request_parser.cpp
.PHONY: request_parser_test

request_test: libgtest.a
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc request_test.cpp libgtest.a $(TEST_FLAGS) -o request_test
	./request_test
	gcov -r request.cpp
.PHONY: request_test

response_test: libgtest.a
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc response_test.cpp libgtest.a $(TEST_FLAGS) -o response_test
	./response_test
	gcov -r response.cpp
.PHONY: response_test

connection_test: libgtest.a
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc connection_test.cpp libgtest.a $(TEST_FLAGS) -o connection_test
	./connection_test
	gcov -r connection.cpp
.PHONY: connection_test

connection_manager_test: libgtest.a
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc connection_manager_test.cpp libgtest.a $(TEST_FLAGS) -o connection_manager_test
	./connection_manager_test
	gcov -r connection_manager.cpp
.PHONY: connection_manager_test

request_handler_file_test: libgtest.a
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc request_handler_file_test.cpp libgtest.a $(TEST_FLAGS) -o request_handler_file_test
	./request_handler_file_test
	gcov -r request_handler_file_test.cpp
.PHONY: request_handler_file_test

mime_types_test: libgtest.a
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc mime_types_test.cpp libgtest.a $(TEST_FLAGS) -o mime_types_test
	./mime_types_test
	gcov -r mime_types_test.cpp
.PHONY: mime_types_test

libgtest.a: $(GTEST_DIR)
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

unittest: http_server_test request_handler_test request_parser_test request_test response_test connection_test connection_manager_test

clean: clean1
	rm -rf http_server_test request_handler_test request_parser_test request_test response_test connection_test connection_manager_test

test: unittest integration-test clean

FLAGS = -l boost_system -lboost_log -pthread -std=c++0x -g -Wall -DBOOST_LOG_DYN_LINK
TEST_FLAGS =-l boost_system -lboost_log -pthread -std=c++0x -g -Wall -fprofile-arcs -ftest-coverage -DBOOST_LOG_DYN_LINK
SOURCE_FILES = http_server.cc nginx-configparser/config_parser.cc webserver_main.cc connection.cc \
connection_manager.cc request_parser.cc response.cc request.cc request_handler_echo.cc \
request_handler_file.cc mime_types.cc
TEST_FILES = http_server.cc nginx-configparser/config_parser.cc connection.cc connection_manager.cc \
 request_parser.cc response.cc request.cc mime_types.cc request_handler_echo.cc request_handler_file.cc
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
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc http_server_test.cc libgtest.a $(TEST_FLAGS) -o http_server_test
	./http_server_test
	gcov -r http_server.cc
.PHONY: http_server_test

request_handler_echo_test: libgtest.a
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc request_handler_echo_test.cc libgtest.a $(TEST_FLAGS) -o request_handler_echo_test
	./request_handler_echo_test
	gcov -r request_handler_echo.cc
.PHONY: request_handler_echo_test

request_parser_test: libgtest.a
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc request_parser_test.cc libgtest.a $(TEST_FLAGS) -o request_parser_test
	./request_parser_test
	gcov -r request_parser.cc
.PHONY: request_parser_test

request_test: libgtest.a
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc request_test.cc libgtest.a $(TEST_FLAGS) -o request_test
	./request_test
	gcov -r request.cc
.PHONY: request_test

response_test: libgtest.a
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc response_test.cc libgtest.a $(TEST_FLAGS) -o response_test
	./response_test
	gcov -r response.cc
.PHONY: response_test

connection_test: libgtest.a
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc connection_test.cc libgtest.a $(TEST_FLAGS) -o connection_test
	./connection_test
	gcov -r connection.cc
.PHONY: connection_test

connection_manager_test: libgtest.a
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc connection_manager_test.cc libgtest.a $(TEST_FLAGS) -o connection_manager_test
	./connection_manager_test
	gcov -r connection_manager.cc
.PHONY: connection_manager_test

request_handler_file_test: libgtest.a
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc request_handler_file_test.cc libgtest.a $(TEST_FLAGS) -o request_handler_file_test
	./request_handler_file_test
	gcov -r request_handler_file_test.cc
.PHONY: request_handler_file_test

mime_types_test: libgtest.a
	g++ -isystem ${GTEST_DIR}/include $(TEST_FILES) ${GTEST_DIR}/src/gtest_main.cc mime_types_test.cc libgtest.a $(TEST_FLAGS) -o mime_types_test
	./mime_types_test
	gcov -r mime_types_test.cc
.PHONY: mime_types_test

libgtest.a: $(GTEST_DIR)
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

unittest: http_server_test request_handler_echo_test request_parser_test request_test response_test connection_test connection_manager_test request_handler_file_test mime_types_test

clean: clean1
	rm -rf http_server_test request_handler_echo_test request_parser_test request_test response_test connection_test connection_manager_test request_handler_file_test mime_types_test

test: unittest integration-test clean

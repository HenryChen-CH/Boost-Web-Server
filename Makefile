FLAGS = -l boost_system -pthread -std=c++0x -g -Wall
SOURCE_FILES = http_server.cpp nginx-configparser/config_parser.cc webserver_main.cc connection.cpp connection_manager.cpp request_handler.cpp request_parser.cpp response.cpp request.cpp

all: compile

compile: $(SOURCE_FILES)
	@g++ $(SOURCE_FILES) $(FLAGS) -o webserver

clean:
	@rm -rf webserver *.dSYM *.o *.a

test: compile
	chmod a+x integration-test.py
	./integration-test.py

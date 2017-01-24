all: compile

compile:
	@g++ http_server.cpp nginx-configparser/config_parser.cc webserver_main.cc  -l boost_system -pthread -std=c++0x -g -Wall -o webserver

clean:
	@rm -rf webserver *.dSYM *.o *.a

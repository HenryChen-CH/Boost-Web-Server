BOOST_FLAGS = -lboost_log_setup -lboost_log -lboost_thread -lboost_system -lboost_regex 
TEST_FLAGS = -fprofile-arcs -ftest-coverage
COMMON_FLAGS = -lpthread

GTEST_DIR = googletest/googletest
CPP_FILES = $(wildcard ./src/*.cc)
OBJ_FILES := $(CPP_FILES:.cc=.o)
NGINX_FILE= nginx-configparser/config_parser.cc
NGINX_OBJ= nginx-configparser/config_parser.o
MARKDOWN_SOURCE = cpp-markdown/src/markdown-tokens.cpp cpp-markdown/src/markdown.cpp
MARKDOWN_OBJ = $(patsubst %.cpp, %.o, $(MARKDOWN_SOURCE))
ZLIB_OBJ = src/library/zlib.o

TEST_FILES = $(wildcard ./test/*_test.cc)
TESTS = $(TEST_FILES:%.cc=%)
CPP_EXCEPT_MAIN = $(filter-out ./src/webserver_main.cc, $(CPP_FILES))
AWS_KEY = ~/Downloads/cs130.pem

webserver: $(OBJ_FILES) $(NGINX_OBJ) $(MARKDOWN_OBJ) $(ZLIB_OBJ)
	$(CXX) -o $@ $^ -static-libgcc -static-libstdc++ $(COMMON_FLAGS) -Wl,-Bstatic $(BOOST_FLAGS) -lz
.PHONY: webserver

cpp-markdown/src/%.o: cpp-markdown/src/%.cpp
	g++ -std=c++11 $^ -c -o $@

nginx-configparser/config_parser.o: $(NGINX_FILE)
	g++ -std=c++11 -g -I ${GTEST_DIR}/include -I . -c -BOOST_ALL_DYN_LINK -o $@ $<

src/library/zlib.o: src/library/zlib.cpp
	g++ -std=c++11 -c $< -o $@

src/%.o: src/%.cc
	g++ -std=c++11 -g -I ${GTEST_DIR}/include -I . -c -BOOST_ALL_DYN_LINK -o $@ $<

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
	./proxy-integration-test.sh
.PHONY: test

proxy:
	./proxy-integration-test.sh

libgtest.a: $(GTEST_DIR)
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

clean:
	@find . -name \*.o -delete
	@find . -name \*.gcov -delete
	@find . -name \*.gcno -delete
	@find . -name \\*.gcda -delete
	@rm -rf webserver *.d *.a

docker-build:
	docker build -t httpserver.build .
	docker run httpserver.build > ./deploy/binary.tar
	tar -xvf ./deploy/binary.tar -C ./deploy
	rm ./deploy/binary.tar
	docker build -t httpserver deploy

docker-deploy: $(AWS_KEY)
	docker save -o ./httpserver.tar httpserver
	scp -i $(AWS_KEY) ./httpserver.tar ubuntu@ec2-52-37-96-79.us-west-2.compute.amazonaws.com:~
	rm ./httpserver.tar
	ssh -i $(AWS_KEY) ubuntu@ec2-52-37-96-79.us-west-2.compute.amazonaws.com \
	-t 'docker stop $$(docker ps -a -q); docker load -i httpserver.tar; docker run -d -t -p 3000:3000 httpserver; exit'
.PHONY: docker-deploy

print-%  : ; @echo $* = $($*)

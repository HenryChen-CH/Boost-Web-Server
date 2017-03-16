# Asynchronous Wed Server
[![Build Status](https://travis-ci.org/HenryChen-CH/Boost-Web-Server.svg?branch=master)](https://travis-ci.org/HenryChen-CH/Boost-Web-Server)
### This is an Extensible Web Server Using Boost Asio Library, supporting HTTP1.1, HTTP Compression and Multi-Threading. 
---
### Usage:

Get the source code:
```
git clone --recursive https://github.com/HenryChen-CH/Boost-Web-Server.git
```

Change into the directory:

```
cd ./Boost-Web-Server
```

Compile:

```
make
```
Tips: When you just change one source file or very few files, you just need to remove the corresponding .o file, and compile again, it will greatly speed up compiling process.  
Run:

```
./webserver <config>   
```
Run tests:

```
make test
```
### Docker
+ Build Docker Image   
`make docker-build`   
we use static linking to build the binary, so that we can shrink the size of docker image to about 10MB!
+ Deploy to ec2(make sure using the correct key and server address in Makefile)   
`make docker-deploy`
+ Visit   
Status Page: http://ec2-52-37-96-79.us-west-2.compute.amazonaws.com/status   
Echo Page: http://ec2-52-37-96-79.us-west-2.compute.amazonaws.com/f/bar   
File: http://ec2-52-37-96-79.us-west-2.compute.amazonaws.com/f/bunny4.png
### How To Add Handler
You can conveniently add Handler extending function of the server.  

* Create your own class. Be sure using .cc .h as file extension  
* Your Handler class should inherit RequestHandler class
* Implement all  virtual functions of RequestHandler class
* call REGISTER_REQUEST_HANDLER(YourClassName) in header file 
* Configure your handler in config file
* All Done, you can compile and run now

### Config File Format
```
listen <port number>;   
path /path HandlerName {   
  HandlerOptions;   
}
```
### Dependency
```
sudo apt-get install libboost-all-dev
sudo apt-get install zlib1g-dev
```

### Based On:
[Boost HTTP Server Example](http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/examples/cpp11_examples.html)

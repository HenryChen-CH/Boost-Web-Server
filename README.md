#Simple Http Server
[![Build Status](https://travis-ci.org/UCLA-CS130/Team13.svg?branch=master)](https://travis-ci.org/UCLA-CS130/Team13)
### Unsynchronous Web Server
##Team Member:
* Hao Chen
* Yunwen Zhu

###Usage:

Get the source code:
```
git clone --recursive https://github.com/UCLA-CS130/Team13.git
```

Change into the directory:

```
cd ./Team13
```

Compile:

```
make
```

Run:

```
./webserver <config>
```
### How To Contribute
You can conveniently add Handler extending function of the server.  

* Create your own class. Be sure using .cc .h as file extension  
* Your Handler class should inherit RequestHandler class
* Implement all  virtual functions of RequestHandler class
* call REGISTER_REQUEST_HANDLER(YourClassName) in header file 
* Configure your handler in config file
* All Done, you can compile and run now

###Based On:
[Boost HTTP Server Example](http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/examples/cpp11_examples.html)

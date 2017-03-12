FROM ubuntu:14.04
RUN apt-get update
RUN apt-get install -y libboost-all-dev
RUN apt-get install -y make
RUN apt-get install -y g++
RUN apt-get install -y zlib1g-dev

WORKDIR /opt/webserver
COPY . /opt/webserver
RUN make clean && make webserver

CMD tar -cf - webserver

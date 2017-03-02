#!/bin/bash

make clean
make

echo "listen 1234;
path /proxy ProxyHandler {
	host localhost;
	port 5678;
}" > proxy_config

echo "listen 5678;
path /echo EchoHandler {}
path /static StaticFileHandler {
  root ./foo/bar;
}" > backend_config

echo "Starting the two web server instances"
./webserver backend_config &>/dev/null &
./webserver proxy_config &>/dev/null &

echo "Issuing static file requests to proxy and backend"
curl -s localhost:5678/static/temp.txt > backend_static
curl -s localhost:1234/proxy/static/temp.txt > proxy_static

echo "Checking that the responses are the same"
diff backend_static proxy_static > diff_static
EXIT_STATUS+=$?

echo "*******************************************************"
if [ "$EXIT_STATUS" -eq 0 ]
then
	echo "SUCCESS: Proxy and backend responses were the same"
else
	echo "FAILURE: Proxy and backend responses differed. The diff is as follows:"
	cat diff_static
fi
echo "*******************************************************"

echo "Cleaning up"
pkill webserver
rm backend_config
rm proxy_config
rm backend_static
rm proxy_static
rm diff_static

exit "$EXIT_STATUS"
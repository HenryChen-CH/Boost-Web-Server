#!/bin/bash

# Start the server
echo "Running a proxy integration test on the server"
echo "listen 3000;
path /echo EchoHandler {}
path /proxytest ProxyHandler {
	host localhost;
	port 3001;
}
" > test_config

echo "listen 3001;
path /f StaticFileHandler {
  root ./foo/bar;
}
" > test_config2

echo "Starting webserver on port 3000..."
./webserver test_config &>/dev/null &
echo "Starting webserver on port 3001..."
./webserver test_config2 &>/dev/null &

echo "Running curl requests..."
#proxy test
curl -i -s localhost:3000/proxytest > test_proxy_echo2
curl -i -s localhost:3001/notfound > test_proxy_echo

EXIT_STATUS=0
diff test_proxy_echo test_proxy_echo2 > diff_proxy_echo_output
num_lines="$(wc -l < diff_proxy_echo_output)"
#assert that no less than 10 lines be different to allow for some minor spacing changes.
if [ "$num_lines" -gt 10 ]
then
  EXIT_STATUS+=1
fi

# Indicate success or failure with an appropriate exit code
# Diff returns 0 if there are no differences, 1 if there are,
# and another number if something else went wrong
if [ "$EXIT_STATUS" -eq 0 ]
then
	echo "SUCCESS: Proxy test produced expected output"
else
	echo "Proxy test did not produce the expected result"
	echo "Diff of curl output:"
	cat diff_proxy_echo_output
fi

# Shutdown the webserver
echo "Cleaning up and shutting down"
pkill webserver
rm test_config
rm test_config2
rm diff_proxy_echo_output
rm test_proxy_echo
rm test_proxy_echo2

exit "$EXIT_STATUS"
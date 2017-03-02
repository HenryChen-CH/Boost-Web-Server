#!/usr/bin/python
from subprocess import call, Popen
import requests, os, httplib
from time import sleep

port = 3000
FNULL = open(os.devnull, 'w')

global http_request_raw

# reference: http://stackoverflow.com/questions/20658572/python-requests-print-entire-http-request-raw
def patch_send():
    old_send= httplib.HTTPConnection.send
    def new_send( self, data ):
        global http_request_raw
        http_request_raw = data
        return old_send(self, data) #return is not necessary, but never hurts, in case the library is changed
    httplib.HTTPConnection.send= new_send
patch_send()

print "Multi Thread Test Start"

server = Popen(['./webserver', 'config'], stdout=FNULL)
sleep(0.1)
telnet_proc = Popen(["telnet", "localhost", "3000"], stdout=FNULL)

headers = {'content-type': 'text/plain'}
r = requests.get('http://localhost:3000/f/temp.txt', headers=headers)

file_content = "something"
if r.status_code == requests.codes.ok:
    if r.headers['content-type'] != "text/plain":
        print "Test fail"
        print "actual content-type is: " + r.headers['content-type']
        print "expected content-type is: "+ "text/plain"
        server.kill()
        telnet_proc.kill()
        exit(1);

    if r.content != file_content:
        print "Test fail"
        print "Request: "
        print file_content
        print "Response content: "
        print r.content
        server.kill()
        telnet_proc.kill()
        exit(1)
else:
    print "status code not 200, test fail"
    server.kill()
    telnet_proc.kill()
    exit(1)

print "Multi Thread Test pass!"

server.kill()
telnet_proc.kill()
exit(0)


# multithread test


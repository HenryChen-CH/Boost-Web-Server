#!/usr/bin/python
from subprocess import call, Popen
import requests, os, httplib

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

server = Popen(['./webserver', 'config'], stdout=FNULL)
headers = {'content-type': 'text/plain'}
r = requests.get('http://localhost:3000', headers=headers)

if r.status_code == requests.codes.ok:
    if r.headers['content-type'] != "text/plain":
        print "Test fail"
        print "actual content-type is: " + r.headers['content-type']
        print "expected content-type is: "+ "text/plain"
        server.kill()
        exit(1);
        
    if r.content != http_request_raw:
        print "Test fail"
        print "Request: "
        print http_request_raw
        print "Response content: "
        print r.content
        server.kill()
        exit(1)
else:
    print "status code not 200, test fail"
    server.kill()
    exit(1)

print "Test pass"

server.kill()
exit(0)

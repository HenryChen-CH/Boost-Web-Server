#!/usr/bin/python
from subprocess import call, Popen
import requests, os

port = 3000
FNULL = open(os.devnull, 'w')

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

    # if r.content != r.request.headers:
    #     print "Test fail"
    #     print "actual request header: "
    #     print r.request.headers
    #     print "response content: "
    #     print r.content
    #     server.kill()
    #     exit(1)

else:
    print "status code not 200, test fail"
    server.kill()
    exit(1)

print "Test pass"

server.kill()
exit(0)

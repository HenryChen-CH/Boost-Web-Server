#!/usr/bin/python
from subprocess import call, Popen
import requests, os, httplib, sys
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

def main(argv=sys.argv): 
    server = Popen(['./webserver', 'config'], stdout=FNULL)
    sleep(0.1)
    num_of_thread = 2
    telnet_procs = []
    if len(argv) == 2:
        num_of_thread = int(argv[1])
    print "run telnet processes num: " + str(num_of_thread)
    for i in range(num_of_thread):
        telnet_procs.append(Popen(["telnet", "localhost", "3000"], stdout=FNULL))

    headers = {'content-type': 'text/plain'}
    r = requests.get('http://localhost:3000/f/temp.txt', headers=headers)

    file_content = "something"
    if r.status_code == requests.codes.ok:
        if r.headers['content-type'] != "text/plain":
            print "Test fail"
            print "actual content-type is: " + r.headers['content-type']
            print "expected content-type is: "+ "text/plain"
            server.kill()
            for proc in telnet_procs:
                proc.kill()
            exit(1);

        if r.content != file_content:
            print "Test fail"
            print "Request: "
            print file_content
            print "Response content: "
            print r.content
            server.kill()
            for proc in telnet_procs:
                proc.kill()
            exit(1)
    else:
        print "status code not 200, test fail"
        server.kill()
        for proc in telnet_procs:
            proc.kill()
        exit(1)

    print "Multi Thread Test pass!"

    server.kill()
    for proc in telnet_procs:
        proc.kill()
    exit(0)

if __name__ == "__main__":
    main()

# multithread test


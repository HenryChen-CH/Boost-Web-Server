#include <iostream>
#include "http_server.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Server start" << endl;
    if (argc != 2) {
        cout << "Please specify config file\n";
        return 1;
    }
    http_server server;

    if (server.Init(string(argv[1]))) {
        server.start();
    };

    return 0;
}

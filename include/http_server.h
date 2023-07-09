#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
//#ifndef VikaHTTPServer
//#define VikaHTTPServer
#include <string>

class HTTPServer {
private:
    std::string versionPrefix_ = "/v0";
    int port_=8888;
    //httplib::Server server_;
public:
    HTTPServer(int port = 8888);
    void run();
};

#endif #HTTP_SERVER_H

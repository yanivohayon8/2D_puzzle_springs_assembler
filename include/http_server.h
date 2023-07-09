#pragma once
#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#include <string>
#include "cpp-httplib/httplib.h"
#include "Puzzle.h"
#include "DataLoader.h"

class HTTPServer {
private:
    std::string versionPrefix_ = "/v0";
    Puzzle puzzle_;
    DataLoader dataLoader_;
    
    void handlePuzzleLoading(const httplib::Request& req, httplib::Response& res);
    //httplib::Server server_;
public:
    int port_=8888;
    HTTPServer(int port = 8888);
    void run(int& i);
};

#endif #HTTP_SERVER_H

#pragma once
#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#include <string>
#include "cpp-httplib/httplib.h"
#include "Utils.h"
//#include <json/json.h>
#include "Puzzle.h"
#include "DataLoader.h"
#include "VertexMating.h"

class HTTPServer {
private:
    std::string versionPrefix_ = "/v0";
    Puzzle puzzle_;
    DataLoader dataLoader_;
    SilentReconstructor silentReconstructor_;
    
    void handlePuzzleLoading(const httplib::Request& req, httplib::Response& res);
    void handleReconstruct(const httplib::Request& req, httplib::Response& res);//, Json::Value& bodyRequest
    //httplib::Server server_;
public:
    int port_=8888;
    HTTPServer(int port = 8888);
    void run();
};

#endif #HTTP_SERVER_H

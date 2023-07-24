#pragma once
#include "http_server.h"
class HttpServerCD :
    public HTTPServer
{
protected:
    DataLoader dataLoader_;
    Puzzle puzzle_;
    void handleReconstruct(const httplib::Request& req, httplib::Response& res, std::string requestBody);//, Json::Value& bodyRequest
    void handlePuzzleLoading(const httplib::Request& req, httplib::Response& res);

public:
    HttpServerCD(int port = 8888);
};


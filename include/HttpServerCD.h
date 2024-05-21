#pragma once
#include "http_server.h"
class HttpServerCD :
    public HTTPServer
{
protected:
    DataLoader dataLoader_;
    Puzzle puzzle_;
    SilentReconstructor silentReconstructor_;
    void handleReconstruct(const httplib::Request& req, httplib::Response& res, std::string requestBody);
    void handlePuzzleLoading(const httplib::Request& req, httplib::Response& res);

public:
    void run();
    HttpServerCD(int port = 8888);
};


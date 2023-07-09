#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#include <string>

class HTTPServer {
private:
    std::string versionPrefix_ = "/v0";
    /*void handlePuzzleLoading(const httplib::Request& req, httplib::Response& res, std::string puzzleDirectory);
    Puzzle puzzle_;*/
    //httplib::Server server_;
public:
    int port_=8888;
    HTTPServer(int port = 8888);
    void run(int& i);
};

#endif #HTTP_SERVER_H

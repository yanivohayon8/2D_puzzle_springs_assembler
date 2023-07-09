#include "http_server.h"
#include "cpp-httplib/httplib.h"

HTTPServer::HTTPServer(int port)
    : port_(port)
{
}

void HTTPServer::run()
{
    httplib::Server server_;
    server_.Get(versionPrefix_+"/sanity", [&](const httplib::Request& req, httplib::Response& res) {

        std::cout << "Received request with params:" << std::endl;
        for (const auto& param : req.params) {
            std::cout << param.first << ": " << param.second << std::endl;
        }

        // Echo the body of the request
        res.status = 200;
        res.set_content("Hello World", "text/plain");
    });

    server_.Post(versionPrefix_+"/simulations", [&](const httplib::Request& req, httplib::Response& res) {

        std::vector<std::string> requestedParams = { "dataset","puzzle" };

        for (auto& param: requestedParams )
        {
            if (!req.has_param(param))
            {
                res.status = 400;
                res.set_content("Please provide the " + param + " parameter", "text/plain");
                return;
            }
        }

        

        res.status = 200;
        res.set_content("<simulation id>", "text/plain");

    });

    std::cout << "HTTP server is listening on port " << port_ << std::endl;
    server_.listen("localhost", port_);
}

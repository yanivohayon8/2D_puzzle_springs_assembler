#include "http_server.h"
#include "cpp-httplib/httplib.h"

HTTPServer::HTTPServer(int port)
    : port_(port)
{
}

void HTTPServer::run()
{
    httplib::Server server_;
    server_.Get("/sanity", [&](const httplib::Request& req, httplib::Response& res) {
        // Echo the body of the request
        res.status = 200;
        res.set_content("Hello World!", "text/plain");
    });

    server_.listen("localhost", port_);
}

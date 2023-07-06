#include <HttpServer.h>


HttpServer::HttpServer(int port)
{
	port_ = port;
}

void HttpServer::run()
{
    server.Get("/sanity", [&](const httplib::Request& req, httplib::Response& res) {
        // Echo the body of the request
        res.status = 200;
        res.set_content("Hello World!", "text/plain");
        });

	server.listen("localhost", port_);
}
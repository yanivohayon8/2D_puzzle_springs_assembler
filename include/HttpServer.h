#pragma once
#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#include <httplib.h>

class HttpServer {

private:
	httplib::Server server;
	int port_;

public:
	HttpServer(int port = 8888);
	void run();
};

#endif #HTTP_SERVER
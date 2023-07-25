#pragma once
#include "http_server.h"

class HttpServerRePAIR : public HTTPServer
{

protected:
	DataLoader dataLoader_;
	void handlePuzzleLoading(const httplib::Request& req, httplib::Response& res, std::string requestBody);
	void handleReconstruct(const httplib::Request& req, httplib::Response& res);
	void handleVisualReconstruct(const httplib::Request& req, httplib::Response& res);

public:
	HttpServerRePAIR(int port = 8888);
	void run();
};
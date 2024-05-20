#pragma once
#include "http_server.h"
#include "RePAIRSilentReconstructor.h"

class HttpServerRePAIR : public HTTPServer
{

protected:
	float boardsize = 10;
	float screenSize = 1380;
	RePAIRSilentReconstructor reconstructor_;//(boardsize, boardsize,screenSize,screenSize);
	DataLoader dataLoader_;
	void handlePuzzleLoading(const httplib::Request& req, httplib::Response& res, std::string requestBody);
	void handleReconstruct(const httplib::Request& req, httplib::Response& res);
	void handleVisualReconstruct(const httplib::Request& req, httplib::Response& res);

public:
	HttpServerRePAIR(int port = 8888);
	void run();
};
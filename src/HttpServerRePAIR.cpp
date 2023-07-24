#include "HttpServerRePAIR.h"


HttpServerRePAIR::HttpServerRePAIR(int port) :HTTPServer(port)
{
	versionPrefix_ = versionPrefix_ + "/RePAIR";
}

void HttpServerRePAIR::handlePuzzleLoading(const httplib::Request& req, httplib::Response& res, std::string requestBody)
{
	payloadToMatings(activeMatings_, requestBody);
	std::set<std::string> fragmentsNames;

	for (auto& mating: activeMatings_)
	{
		fragmentsNames.insert(mating.firstPieceId_);
		fragmentsNames.insert(mating.secondPieceId_);
	}

	activePieces_.clear();
}

void HttpServerRePAIR::handleReconstruct(const httplib::Request& req, httplib::Response& res)
{

}

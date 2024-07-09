#pragma once
#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#include <string>
#include "cpp-httplib/httplib.h"
#include "Utils.h"
//#include <json/json.h>
#include "Puzzle.h"
#include "DataLoader.h"
#include "VertexMating.h"
#include "Piece.h"
#include "Global_Parameters.h"
#include <json.hpp>
#include "reconstruction.h"
#include "RePAIRSilentReconstructor.h"
#include "SilentReconstructor.h"

class HTTPServer {

protected:
    virtual void handleReconstruct(const httplib::Request& req, httplib::Response& res, std::string requestBody);//, Json::Value& bodyRequest
    virtual void handlePuzzleLoading(const httplib::Request& req, httplib::Response& res, std::string requestBody);


    void loadMatings_(float coordinatesScale);
    void loadPieces_(float coordinatesScale);

    std::vector<Piece> activePieces_;
    std::vector<VertexMating*> activeMatings_;
    httplib::Server server_;
    std::string versionPrefix_ = "/v1";

    nlohmann::json currentRequestBody_;
    httplib::Request currentRequest_;

    SilentReconstructor* silentReconstructor_;

public:
    int port_=8888;
    HTTPServer(int port = 8888);
    void run();
    void payloadToMatings(std::vector<VertexMating*>& oMatings,std::string requestBody);
    nlohmann::json buildSpringsJson(std::vector<VertexMating*>& matings); //
    nlohmann::json buildPieceCartesianJson(std::map<std::string, std::vector<b2Vec2>>* &pieces2Coords);
    nlohmann::json buildPieceTransformationJson(std::map<std::string, std::pair<float, b2Vec2>>& piece2FinalTransformation);

    void loadPuzzleData();
    //void initPuzzle();
    void initReconstruction();
    void reconstruct();
};

#endif #HTTP_SERVER_H

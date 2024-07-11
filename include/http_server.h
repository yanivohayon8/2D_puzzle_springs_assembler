#pragma once
#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#include <string>
#include "cpp-httplib/httplib.h"
#include "Utils.h"
#include "VertexMating.h"
#include "Piece.h"
#include "Global_Parameters.h"
#include <json.hpp>
#include "reconstruction.h"
#include "SilentReconstructor.h"
#include "VisualReconstructor.h"

class HTTPServer {

protected:

    void updateBoardDimensions();
    std::string currentBoardDimensionsConfig="";
    float largeBoardSizeLength_ = 80;
    float mediumBoardSizeLength_ = 40;
    float smallBoardSizeLength_ = 10;
    float defaultScreenLength = 1380;

    void loadMatings_(float coordinatesScale);
    void loadPieces_(float coordinatesScale);

    std::vector<Piece> inputtedPieces_;
    std::vector<VertexMatingRePAIR*> inputtedMatings_;
    httplib::Server server_;
    std::string versionPrefix_ = "/v1";

    nlohmann::json currentRequestBody_;
    httplib::Request currentRequest_;

    Reconstructor* reconstructor_;

public:
    int port_=8888;
    HTTPServer(int port = 8888);
    void run();
    void loadPuzzleData(float coordinatesScale);    
    void initReconstruction();
    nlohmann::json reconstruct(float coordinatesScale);
};

#endif #HTTP_SERVER_H

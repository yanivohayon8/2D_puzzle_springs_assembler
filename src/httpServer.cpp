#include "httpServer.h"

HTTPServer::HTTPServer(int port)
    : port_(port)
{
}

float HTTPServer::findLongestPieceEdgeLength_()
{
    auto& piecesJson = currentRequestBody_["pieces"];
    float maxLength = 0;

    for (auto& pieceIt = piecesJson.begin(); pieceIt != piecesJson.end(); ++pieceIt)
    {
        auto& pieceJson = pieceIt.value();
        std::vector<b2Vec2> coordinates;

        auto& prevCoord = pieceJson["polygon"].back();
        float prevX = static_cast<float>(prevCoord.at(0));
        float prevY = static_cast<float>(prevCoord.at(1));
        float length;

        for (auto& coordIt = pieceJson["polygon"].begin(); coordIt != pieceJson["polygon"].end(); ++coordIt)
        {
            float x_ = static_cast<float>(coordIt->at(0));
            float y_ = static_cast<float>(coordIt->at(1));
            length = sqrt((prevX - x_) * (prevX - x_) + (prevY - y_) * (prevY - y_));

            if (maxLength < length)
            {
                maxLength = length;
            }
        }
    }

    return maxLength;
}


float HTTPServer::findPieceScale_()
{
    float longestEdge = findLongestPieceEdgeLength_();
    float ratioLongest2BoardSize = 1.0 / 4;
    float boardHeight = reconstructor_->boardHeight_;
    float scale = boardHeight / longestEdge * ratioLongest2BoardSize;

    return scale;
}

void HTTPServer::updateBoardDimensions()
{

    if (currentRequest_.has_param("boardSize"))
    {
        std::string boardSize = currentRequest_.get_param_value("boardSize");

        if (boardSize == "large" && currentBoardDimensionsConfig != "large")
        {
            currentBoardDimensionsConfig = "large";
            reconstructor_->setBoardHeight(largeBoardSizeLength_);
            reconstructor_->setBoardWidth(largeBoardSizeLength_);
            reconstructor_->updateScreen();
        }

        if (boardSize == "medium" && currentBoardDimensionsConfig != "medium")
        {
            currentBoardDimensionsConfig = "medium";
            reconstructor_->setBoardHeight(mediumBoardSizeLength_);
            reconstructor_->setBoardWidth(mediumBoardSizeLength_);
            reconstructor_->updateScreen();
        }

        if (boardSize == "small" && currentBoardDimensionsConfig != "small")
        {
            currentBoardDimensionsConfig = "small";
            reconstructor_->setBoardHeight(smallBoardSizeLength_);
            reconstructor_->setBoardWidth(smallBoardSizeLength_);
            reconstructor_->updateScreen();
        }

        return;
    }

    bool isUpdated = false;

    if (currentRequest_.has_param("boardHeight"))
    {
        reconstructor_->setBoardHeight(std::stof(currentRequest_.get_param_value("boardHeight")));
        isUpdated = true;
    }

    if (currentRequest_.has_param("boardWidth"))
    {
        reconstructor_->setBoardWidth(std::stof(currentRequest_.get_param_value("boardWidth")));
        isUpdated = true;
    }

    if (currentRequest_.has_param("screenHeight"))
    {
        reconstructor_->setScreenHeight(std::stof(currentRequest_.get_param_value("screenHeight")));
        isUpdated = true;
    }

    if (currentRequest_.has_param("screenWidth"))
    {
        reconstructor_->setScreenWidth(std::stof(currentRequest_.get_param_value("screenWidth")));
        isUpdated = true;
    }

    if (isUpdated)
    {
        reconstructor_->updateScreen();
    }
}

void HTTPServer::loadMatings_(float coordinatesScale)
{
    inputtedMatings_.clear();
    auto& matingsJson = currentRequestBody_["matings"];

    for (auto& matingIt = matingsJson.begin(); matingIt != matingsJson.end(); ++matingIt)
    {
        auto& matingJson = matingIt.value();

        std::string firstPieceId = matingJson["firstPiece"];
        double firstPieceLocalCoordsX = matingJson["firstPieceLocalCoords"].at(0);
        double firstPieceLocalCoordsY = matingJson["firstPieceLocalCoords"].at(1);
        b2Vec2 firstPieceLocalCoords(firstPieceLocalCoordsX * coordinatesScale,
            firstPieceLocalCoordsY * coordinatesScale);

        std::string secondPieceId = matingJson["secondPiece"];
        double secondPieceLocalCoordsX = matingJson["secondPieceLocalCoords"].at(0);
        double secondPieceLocalCoordsY = matingJson["secondPieceLocalCoords"].at(1);
        b2Vec2 secondPieceLocalCoords(secondPieceLocalCoordsX * coordinatesScale,
            secondPieceLocalCoordsY * coordinatesScale);

        VertexMating* mating = new VertexMating(firstPieceId, firstPieceLocalCoords, secondPieceId, secondPieceLocalCoords);
        inputtedMatings_.push_back(mating);
    }
}

void HTTPServer::loadPieces_(float coordinatesScale)
{
    inputtedPieces_.clear();

    auto& piecesJson = currentRequestBody_["pieces"];

    for (auto& pieceIt = piecesJson.begin(); pieceIt != piecesJson.end(); ++pieceIt)
    {
        auto& pieceJson = pieceIt.value();
        std::vector<b2Vec2> coordinates;

        for (auto& coordIt = pieceJson["polygon"].begin(); coordIt != pieceJson["polygon"].end(); ++coordIt)
        {
            float x_ = static_cast<float>(coordIt->at(0)) * coordinatesScale;
            float y_ = static_cast<float>(coordIt->at(1)) * coordinatesScale;
            coordinates.push_back(b2Vec2{ x_,y_ });
        }

        std::string imagePath = "";

        if (pieceJson.contains("imagePath"))
        {
            imagePath = pieceJson["imagePath"];
        }

       
        Piece piece(pieceJson["id"], coordinates, imagePath);

        if (pieceJson.contains("fixedRotationAngle"))
        {
            piece.setIsRotationFixed(true);
            piece.setInitialAngle(pieceJson["fixedRotationAngle"]);
        }

        inputtedPieces_.push_back(piece);
    }
}

void HTTPServer::loadPuzzleData(float coordinatesScale)
{
    loadMatings_(coordinatesScale);
    loadPieces_(coordinatesScale);

}

void HTTPServer::run()
{
    server_.Get(versionPrefix_ + "/sanity", [&](const httplib::Request& req, httplib::Response& res) {
        res.status = 200;
        res.set_content("Hello World!", "text/plain");
    });


    server_.Post(versionPrefix_ + "/reconstructions", [&](const httplib::Request& req, httplib::Response& res, const httplib::ContentReader& content_reader) {

        try
        {

            std::string strRequestBody;
            content_reader([&](const char* data, size_t dataLength) {strRequestBody.append(data, dataLength); return true;});
            currentRequestBody_ = nlohmann::json::parse(strRequestBody);
            currentRequest_ = req;
        
            if (currentRequest_.has_param("interactiveOn"))
            {
                reconstructor_ = new InteractiveReconstructor(smallBoardSizeLength_, smallBoardSizeLength_,
                                                                defaultScreenLength, defaultScreenLength);
            }
            else
            {
                if (currentRequest_.has_param("collideOff"))
                {
                    reconstructor_ = new OffCollideSilentReconstructor(smallBoardSizeLength_, smallBoardSizeLength_,
                                                                defaultScreenLength, defaultScreenLength);
                }
                else
                {
                    reconstructor_ = new OffOnCollideSilentReconstructor(smallBoardSizeLength_, smallBoardSizeLength_,
                        defaultScreenLength, defaultScreenLength);
                }
            }


        
            updateBoardDimensions();
            float pieceScale = findPieceScale_();

            loadPuzzleData(pieceScale);//SCALE_IMAGE_COORDINATES_TO_BOX2D);
            reconstructor_->initRun(currentRequest_, inputtedPieces_, inputtedMatings_);
            nlohmann::json output = reconstructor_->reconstruct(pieceScale);//SCALE_IMAGE_COORDINATES_TO_BOX2D);

            if (currentRequest_.has_param("finalScreenShotPath"))
            {
                reconstructor_->saveScreenShot(currentRequest_.get_param_value("finalScreenShotPath"));
            }

            reconstructor_->closeRun();


            res.status = 200;
            res.set_content(output.dump(), "application/json");
            res.status = 200;
            
            delete reconstructor_;
        }
        catch (const std::exception& ex)
        {
            std::string mess = ex.what();
            std::cout << "Error: " << mess << std::endl;
            throw ex;
        }

    });

    std::cout << "HTTP server is listening on port " << port_ << std::endl;
    server_.listen("localhost", port_);
}
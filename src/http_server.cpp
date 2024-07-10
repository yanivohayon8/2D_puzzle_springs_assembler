#include "http_server.h"

HTTPServer::HTTPServer(int port)
    : port_(port)
{
}

void HTTPServer::payloadToMatings(std::vector<VertexMating*>& oMatings, std::string requestBody)
{
    size_t pos = 0;
    std::string recordsDelimiter = "\r\n"; // ";"
    requestBody += recordsDelimiter;// to include the last record if there is no delimiter there

    while (pos < requestBody.length())
    {
        size_t semicolonm_pos = requestBody.find(recordsDelimiter, pos);

        if (semicolonm_pos == std::string::npos)
            break;

        std::string record = requestBody.substr(pos, semicolonm_pos - pos);
        size_t prev_colum_pos = 0;
        size_t next_colum_pos = record.find(",");
        std::vector<std::string> matingValues;
        std::string val;

        while (next_colum_pos != std::string::npos)
        {
            val = record.substr(prev_colum_pos, next_colum_pos - prev_colum_pos);
            //std::cout << val << std::endl;
            matingValues.push_back(val);
            prev_colum_pos = next_colum_pos + 1;//+1;
            next_colum_pos = record.find(",", prev_colum_pos);
        }

        val = record.substr(prev_colum_pos, record.length() - prev_colum_pos);
        matingValues.push_back(val);

        if (matingValues.size() == 4)
        {
            //VertexMating mating(matingValues[0], std::stoi(matingValues[1]), matingValues[2], std::stoi(matingValues[3]));
            VertexMating* mating = new VertexMating(matingValues[0], std::stoi(matingValues[1]), matingValues[2], std::stoi(matingValues[3]));

            oMatings.push_back(mating);
        }

        pos += record.length() + recordsDelimiter.length();
    }
}

nlohmann::json HTTPServer::buildSpringsJson(std::vector<VertexMating*>& matings)
{
    nlohmann::json joints = nlohmann::json::array();
    float maxLength = 0;
    double sumLengths = 0;

    for (auto& mating : matings)
    {
        auto matingJson = mating->toJson(SCALE_IMAGE_COORDINATES_TO_BOX2D);
        joints.push_back(matingJson);
        sumLengths += matingJson["snapshotedLength"];

        if (matingJson["snapshotedLength"] > maxLength)
        {
            maxLength = matingJson["snapshotedLength"];
        }
    }

    nlohmann::json afterCollision;
    afterCollision["springs"] = joints;
    afterCollision["sumSpringsLength"] = sumLengths;

    return afterCollision;
}

nlohmann::json HTTPServer::buildPieceCartesianJson(std::map<std::string, std::vector<b2Vec2>>* &pieces2Coords)
{
    nlohmann::json piecesJson = nlohmann::json::array();    

    for (auto& pieceIt = pieces2Coords->begin(); pieceIt != pieces2Coords->end(); ++pieceIt)
    {
        nlohmann::json pieceJson;
        pieceJson["pieceId"] = pieceIt->first;
        nlohmann::json coords = nlohmann::json::array();

        for (auto& coord : pieces2Coords->at(pieceIt->first))
        {
            auto x = coord.x / SCALE_IMAGE_COORDINATES_TO_BOX2D;
            auto y = coord.y / SCALE_IMAGE_COORDINATES_TO_BOX2D;
            coords.push_back(nlohmann::json::array({ x,y }));
        }

        pieceJson["coordinates"] = coords;
        piecesJson.push_back(pieceJson);
    }

    return piecesJson;
}

nlohmann::json HTTPServer::buildPieceTransformationJson(std::map<std::string, std::pair<float, b2Vec2>>& piece2FinalTransformation)
{
    nlohmann::json piecesJson = nlohmann::json::array();

    for (auto& pieceIt = piece2FinalTransformation.begin(); pieceIt != piece2FinalTransformation.end(); ++pieceIt)
    {
        nlohmann::json pieceJson;
        pieceJson["pieceId"] = pieceIt->first;
        pieceJson["rotationRadians"] = piece2FinalTransformation.at(pieceIt->first).first;
        
        b2Vec2& translateVector = piece2FinalTransformation.at(pieceIt->first).second;
        pieceJson["translateVectorX"] = translateVector.x / SCALE_IMAGE_COORDINATES_TO_BOX2D;
        pieceJson["translateVectorY"] = translateVector.y / SCALE_IMAGE_COORDINATES_TO_BOX2D;
        piecesJson.push_back(pieceJson);
    }

    return piecesJson;
}


//void load


void HTTPServer::updateBoardDimensions()
{

    if (currentRequest_.has_param("boardSize"))
    {
        std::string boardSize = currentRequest_.get_param_value("boardSize");
        
        if (boardSize == "large" && currentBoardDimensionsConfig !="large")
        {
            currentBoardDimensionsConfig = "large";
            reconstructor_->setBoardHeight(largeBoardSizeLength_);
            reconstructor_->setBoardWidth(largeBoardSizeLength_);
            reconstructor_->updateScreen();
        }
        
        if (boardSize == "medium" && currentBoardDimensionsConfig !="medium")
        {
            currentBoardDimensionsConfig = "medium";
            reconstructor_->setBoardHeight(mediumBoardSizeLength_);
            reconstructor_->setBoardWidth(mediumBoardSizeLength_);
            reconstructor_->updateScreen();
        }
        
        if (boardSize == "small" && currentBoardDimensionsConfig !="small")
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




void HTTPServer::handlePuzzleLoading(const httplib::Request& req, httplib::Response& res, std::string requestBody)
{

}

void HTTPServer::handleReconstruct(const httplib::Request& req, httplib::Response& res, std::string requestBody)
{

}




void HTTPServer::loadMatings_(float coordinatesScale)
{
    activeMatings_.clear();
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

        VertexMatingRePAIR* mating = new VertexMatingRePAIR(firstPieceId, firstPieceLocalCoords, secondPieceId, secondPieceLocalCoords);
        activeMatings_.push_back(mating);
    }
}

void HTTPServer::loadPieces_(float coordinatesScale)
{
    activePieces_.clear();

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

        activePieces_.push_back(piece);
    }
}

void HTTPServer::loadPuzzleData(float coordinatesScale)
{
    loadMatings_(coordinatesScale);
    loadPieces_(coordinatesScale);

}

void HTTPServer::initReconstruction()
{
    reconstructor_->initBoundaryWallBodies();

    reconstructor_->activePieces_ = activePieces_;
    std::string fixedPieceId = "";

    if (currentRequest_.has_param("fixPiece"))
    {
        Piece* fixedPiece = reconstructor_->getMaxMatingsPiece();
        fixedPieceId = fixedPiece->id_;
    }

    std::vector<b2Vec2> positions;

    if (currentRequest_.has_param("seedInitialPositions"))
    {
        int positionPadding = 2;
        int seedPositions = std::stoi(currentRequest_.get_param_value("seedInitialPositions"));

        generate2DVectors(positions, activePieces_.size(), reconstructor_->boardWidth_, reconstructor_->boardHeight_,
            positionPadding, seedPositions);

        std::sort(positions.begin(), positions.end(),
            [](const b2Vec2& a, const b2Vec2& b)->bool {
            return a.y > b.y || (a.y == b.y && a.x > b.x);
            });
    }
    else
    {
        for (int i = 0; i < activePieces_.size(); i++)
        {
            positions.push_back(b2Vec2(reconstructor_->boardWidth_ / 2, reconstructor_->boardHeight_ / 2));
        }
    }

    reconstructor_->initPiecesBodies(activePieces_, fixedPieceId, positions);

    reconstructor_->disableJointsCollide();

    if (currentRequest_.has_param("enableJointsCollide"))
    {
        reconstructor_->enableJointsCollide();
    }

    reconstructor_->initMatingsJoints(activeMatings_);
}

nlohmann::json HTTPServer::reconstruct(float coordinatesScale)
{
    ////silentReconstructor_->

    //silentReconstructor_->setDebugScreenVisibility(false);

    //if (currentRequest_.has_param("visibilityOn"))
    //{
    //    silentReconstructor_->setDebugScreenVisibility(true);
    //    silentReconstructor_->initScreen();
    //}

    //silentReconstructor_->applyImpulseOnBodies(silentReconstructor_->initPowerMagnitude_);

    nlohmann::json output;

    //if (currentRequest_.has_param("collideOff"))
    //{
    //    output = silentReconstructor_->RunOffCollide(coordinatesScale);
    //}
    //else
    //{
    //    output = silentReconstructor_->RunOffOnCollide(coordinatesScale);
    //}

    //silentReconstructor_->screen_->closeWindow();
    //silentReconstructor_->closeRun();

    return output;
}


void HTTPServer::run()
{
    server_.Get(versionPrefix_ + "/sanity", [&](const httplib::Request& req, httplib::Response& res) {
        res.status = 200;
        res.set_content("Hello World!", "text/plain");
    });


    server_.Post(versionPrefix_ + "/reconstructions", [&](const httplib::Request& req, httplib::Response& res, const httplib::ContentReader& content_reader) {

        std::string strRequestBody;
        content_reader([&](const char* data, size_t dataLength) {strRequestBody.append(data, dataLength); return true;});
        currentRequestBody_ = nlohmann::json::parse(strRequestBody);
        currentRequest_ = req;

        
        if (currentRequest_.has_param("interactiveOn"))
        {
            reconstructor_ = new VisualReconstructor(smallBoardSizeLength_, smallBoardSizeLength_,
                                                            defaultScreenLength, defaultScreenLength);
        }
        else
        {
            reconstructor_ = new SilentReconstructor(smallBoardSizeLength_, smallBoardSizeLength_,
                                                            defaultScreenLength, defaultScreenLength);
        }

        updateBoardDimensions();

        try
        {
            loadPuzzleData(SCALE_IMAGE_COORDINATES_TO_BOX2D);
            initReconstruction();
            //nlohmann::json& output = reconstruct(SCALE_IMAGE_COORDINATES_TO_BOX2D);
            nlohmann::json output = reconstructor_->reconstruct();
            delete reconstructor_;

            res.status = 200;
            res.set_content(output.dump(), "application/json");
            res.status = 200;

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
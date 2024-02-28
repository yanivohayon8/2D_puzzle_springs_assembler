#include "HttpServerCD.h"


HttpServerCD::HttpServerCD(int port):HTTPServer(port)
{
    versionPrefix_ = versionPrefix_ + "/ConvexDrawing";
}

void HttpServerCD::handlePuzzleLoading(const httplib::Request& req, httplib::Response& res)
{
    std::string puzzleDirectory = "";
    
    std::string db;
    std::string puzzleNum;
    std::string noise;
    std::vector<std::string> requestedConvexDrawParams = { "db","num","noise" };

    for (auto& param : requestedConvexDrawParams)
    {
        if (!req.has_param(param))
        {
            res.status = 400;
            res.set_content("Please provide the " + param + " parameter", "text/plain");
            return;
        }
    }

    db = req.get_param_value("db");
    puzzleNum = req.get_param_value("num");
    noise = req.get_param_value("noise");

    //puzzleDirectory = "../data/ofir/" + directory + "/Puzzle" + puzzleNum + "/" + noise;
    // 
    //puzzleDirectory = "C:/Users/97254/Desktop/msc/Thesis/ConvexDrawingDataset/" + directory + "/Puzzle" + puzzleNum + "/" + noise;
    puzzleDirectory = "C:/Users/97254/Desktop/msc/Thesis/ConvexDrawingDataset/DB" + db + "/Puzzle" + puzzleNum + "/noise_" + noise;

    std::cout << "Recieved simulation for puzzle in " << puzzleDirectory << std::endl;

    if (puzzleDirectory == dataLoader_.puzzleDirectoryPath_)
    {
        std::cout << "Puzzle already loaded" << std::endl;
        return;
    }

    dataLoader_.setPuzzleDirectory(puzzleDirectory);
    std::vector<Piece> tmpPieces;
    dataLoader_.loadPieces(tmpPieces);
    puzzle_.setPieces(tmpPieces);
}

void HttpServerCD::handleReconstruct(const httplib::Request& req, httplib::Response& res, std::string requestBody)//, Json::Value& bodyRequest
{
    std::string screenShotName = "";
    std::string imageBeforeCollide = "";
    std::string imageAfterCollide = "";

    if (req.has_param("screenShotName"))
    {
        screenShotName = req.get_param_value("screenShotName");

        imageBeforeCollide = dataLoader_.puzzleDirectoryPath_ + "/screenshots/" + screenShotName + "_before_collide.png";
        imageAfterCollide = dataLoader_.puzzleDirectoryPath_ + "/screenshots/" + screenShotName + "_after_collide.png";
    }

    silentReconstructor_.setDebugScreenVisibility(false);
    if (req.has_param("debugVisibily"))
    {
        silentReconstructor_.setDebugScreenVisibility(true);
    }


    std::cout << "Loading Matings" << std::endl;

    activeMatings_.clear();
    activePieces_.clear();


    payloadToMatings(activeMatings_, requestBody);
    puzzle_.findPiecesToReconstruct(activePieces_, activeMatings_);
    std::cout << "Running Reconstructor" << std::endl;
    silentReconstructor_.initRun(activePieces_, activeMatings_);
    silentReconstructor_.Run(imageBeforeCollide, imageAfterCollide);

    nlohmann::json output;
    auto piece2CoordBeforeCollision = silentReconstructor_.getPiece2CoordsBeforeEnableCollision();
    output["piecesBeforeEnableCollision"] = buildPieceCartesianJson(piece2CoordBeforeCollision);
    auto piece2FinalCoord = silentReconstructor_.getPiece2FinalCoords();
    output["piecesFinalCoords"] = buildPieceCartesianJson(piece2FinalCoord);
    output["AfterEnableCollision"] = buildSpringsJson(silentReconstructor_.activeMatings_);

    std::map<std::string, std::pair<float, b2Vec2>> piece2FinalTransformation;
    silentReconstructor_.getPiece2FinalTransformation(piece2FinalTransformation);
    output["piecesFinalTransformation"] = buildPieceTransformationJson(piece2FinalTransformation);

    silentReconstructor_.closeRun();
    activeMatings_.clear();

    res.set_content(output.dump(), "application/json");
    res.status = 200;
}

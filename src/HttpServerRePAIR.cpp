#include "HttpServerRePAIR.h"


HttpServerRePAIR::HttpServerRePAIR(int port) :HTTPServer(port)
{
	versionPrefix_ = versionPrefix_ + "/RePAIR";
	dataLoader_.setPuzzleDirectory("../data/RePAIR");
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
	for (auto& name:fragmentsNames)
	{
		std::string pieceFile = dataLoader_.puzzleDirectoryPath_ + "/csv/" + name + ".csv";
		dataLoader_.loadPieces(activePieces_, true,pieceFile);
	}
}

void HttpServerRePAIR::handleVisualReconstruct(const httplib::Request& req, httplib::Response& res)
{
    std::cout << "Start new reconstruction" << std::endl;
    VisualReconstructor vsReconstructor;
    vsReconstructor.init();
    //vsReconstructor.enableJointsCollide();
    vsReconstructor.setJointStartLength(0.05);
    vsReconstructor.initRun(activePieces_, activeMatings_, 1);//,1
    vsReconstructor.setPiecesCollisionOn();
    vsReconstructor.setPiecesLinearDamping(1);
    vsReconstructor.Run();
    vsReconstructor.closeRun();

    activeMatings_.clear();
    activePieces_.clear();
    res.set_content("Ran simulation", "text/plain");
    res.status = 200;
}


void HttpServerRePAIR::handleReconstruct(const httplib::Request& req, httplib::Response& res)
{
    std::cout << "Start new reconstruction" << std::endl;
    std::string screenShotName = "";
    std::string imageBeforeCollide = "";
    std::string imageAfterCollide = "";

    if (req.has_param("screenShotName"))
    {
        screenShotName = req.get_param_value("screenShotName");

        imageBeforeCollide = dataLoader_.puzzleDirectoryPath_ + "/screenshots/" + screenShotName + "_before_collide.png";
        imageAfterCollide = dataLoader_.puzzleDirectoryPath_ + "/screenshots/" + screenShotName + "_after_collide.png";
    }

    reconstructor_.setDebugScreenVisibility(false);
    if (req.has_param("debugVisibily"))
    {
        reconstructor_.setDebugScreenVisibility(true);
    }

    reconstructor_.setIterToConvBeforeCollide(1000);
    reconstructor_.setIterToConvAfterCollide(2000);
    
    
    reconstructor_.initRun(activePieces_, activeMatings_,1);//,1
    reconstructor_.setPiecesLinearDamping(1);
    reconstructor_.Run(imageBeforeCollide, imageAfterCollide);

    nlohmann::json output;
    auto piece2CoordBeforeCollision = reconstructor_.getPiece2CoordsBeforeEnableCollision();
    output["piecesBeforeEnableCollision"] = buildPieceCartesianJson(piece2CoordBeforeCollision);
    auto piece2FinalCoord = reconstructor_.getPiece2FinalCoords();
    output["piecesFinalCoords"] = buildPieceCartesianJson(piece2FinalCoord);
    output["AfterEnableCollision"] = buildSpringsJson(reconstructor_.activeMatings_);

    reconstructor_.closeRun();
    activeMatings_.clear();
    activePieces_.clear();

    res.set_content(output.dump(), "application/json");
    res.status = 200;
}

void HttpServerRePAIR::run()
{
    //silentReconstructor_.init();
    reconstructor_.init();

    server_.Get(versionPrefix_ + "/sanity", [&](const httplib::Request& req, httplib::Response& res) {

        // Echo the body of the request
        res.status = 200;
        res.set_content("Hello World from RePAIR HTTP server", "text/plain");
    });

 
    server_.Post(versionPrefix_ + "/reconstructions", [&](const httplib::Request& req, httplib::Response& res, const httplib::ContentReader& content_reader) {


        // This is for tests
        bool isVisualTest= req.has_param("isVisualTest");
 

        // For simplicity of development the scheme is 
        // firstPieceId,firstPieceVertexIndex,secondPieceId,secondPieceVertexIndex;
        std::string strRequestBody;

        content_reader([&](const char* data, size_t dataLength) {
            strRequestBody.append(data, dataLength);
            return true;
            });

        try
        {
            handlePuzzleLoading(req, res,strRequestBody);

            if (isVisualTest)
            {
                handleVisualReconstruct(req, res);
            }
            else
            {
                handleReconstruct(req, res);
            }
        }
        catch (const std::exception& ex)
        {
            std::string mess = ex.what();
            std::cout << "Error: " << mess << std::endl;
            throw ex;
        }
        });

    std::cout << "RePAIR HTTP server is listening on port " << port_ << std::endl;
    server_.listen("localhost", port_);
}

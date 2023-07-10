#include "http_server.h"

HTTPServer::HTTPServer(int port)
    : port_(port)
{
}

void HTTPServer::handlePuzzleLoading(const httplib::Request& req, httplib::Response& res)
{
    std::string puzzleDirectory="";
    std::vector<std::string> requestedParams = { "dataset" };

    for (auto& param : requestedParams)
    {
        if (!req.has_param(param))
        {
            res.status = 400;
            res.set_content("Please provide the " + param + " parameter", "text/plain");
            return;
        }
    }

    if (req.get_param_value("dataset") == "ConvexDrawing")
    {
        std::string directory;
        std::string puzzleNum;
        std::string noise;
        std::vector<std::string> requestedConvexDrawParams = { "image","num","noise"};

        for (auto& param : requestedConvexDrawParams)
        {
            if (!req.has_param(param))
            {
                res.status = 400;
                res.set_content("Please provide the " + param + " parameter", "text/plain");
                return;
            }
        }

        directory = req.get_param_value("image");
        puzzleNum = req.get_param_value("num");
        noise = req.get_param_value("noise");
       
        puzzleDirectory = "../data/ofir/" + directory + "/Puzzle" + puzzleNum + "/" + noise;
    }

    if (puzzleDirectory=="")
    {
        res.status = 400;
        std::string mess = "Could not find puzzle " + req.get_param_value("puzzle") + 
                            " under dataset" + req.get_param_value("dataset");
        res.set_content(mess, "text/plain");
    }

    std::cout << "Recieved simulation for puzzle in " << puzzleDirectory << std::endl;

    dataLoader_.setPuzzleDirectory(puzzleDirectory);

    std::vector<Piece> tmpPieces;
    std::vector<VertexMating*> tmpMatings;
    dataLoader_.loadVertexMatings(tmpMatings, "springs_anchors_correct.csv");
    dataLoader_.loadPieces(tmpPieces);    
    puzzle_.setGroundTruthMatings(tmpMatings);
    puzzle_.setPieces(tmpPieces);

    res.status = 200;
    res.set_content("Puzzle Loaded", "text/plain");
}

void HTTPServer::handleReconstruct(const httplib::Request& req, httplib::Response& res,std::string requestBody)//, Json::Value& bodyRequest
{
    std::cout << "Loading Matings" << std::endl;
    
    activeMatings_.clear();
    activePieces_.clear();
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
        //std::cout << val << std::endl;

        if (matingValues.size() == 4)
        {
            VertexMating* mating = new VertexMating(matingValues[0], std::stoi(matingValues[1]),
                matingValues[2], std::stoi(matingValues[3]));

            activeMatings_.push_back(mating);
        }

        pos += record.length() + recordsDelimiter.length();
    }

    //puzzle_.findPiecesToReconstruct()
       /*puzzle.findPiecesToReconstruct(activePieces, trueMatings)
       silentReconstructor.initRun(activePieces, matings);
       silentReconstructor.Run("../data/deleteme.png");
       silentReconstructor.closeRun()*/
    
    //puzzle_.findPiecesToReconstruct(pieces, matings);
    //silentReconstructor_.initRun(pieces, matings);

    puzzle_.findPiecesToReconstruct(activePieces_, activeMatings_);
    std::cout << "Running Reconstructor" << std::endl;
    silentReconstructor_.initRun(activePieces_, activeMatings_);
    silentReconstructor_.Run(dataLoader_.puzzleDirectoryPath_ + "/assembly.png");
    silentReconstructor_.closeRun();

    for (auto& mating: activeMatings_)
    {
        delete &mating;
    }

    activeMatings_.clear();

    res.status = 200;
    res.set_content("Vika", "text/plain");
}

void HTTPServer::run()
{
    silentReconstructor_.init();

    server_.Get(versionPrefix_+"/sanity", [&](const httplib::Request& req, httplib::Response& res) {

        std::cout << "Received request with params:" << std::endl;
        for (const auto& param : req.params) {
            std::cout << param.first << ": " << param.second << std::endl;
        }

        // Echo the body of the request
        res.status = 200;
        res.set_content("Hello World", "text/plain");
    });

    server_.Post(versionPrefix_ + "/simulations", [&](const httplib::Request & req, httplib::Response & res) {
        handlePuzzleLoading(req, res);
    });

    server_.Post(versionPrefix_ + "/reconstructions", [&](const httplib::Request& req, httplib::Response& res, const httplib::ContentReader& content_reader) {
        // For simplicity of development the scheme is 
        // firstPieceId,firstPieceVertexIndex,secondPieceId,secondPieceVertexIndex;
        std::string strRequestBody;

        content_reader([&](const char* data, size_t dataLength) {
            strRequestBody.append(data, dataLength);
            return true;
        });
        
        try
        {

            handleReconstruct(req, res, strRequestBody);
        }
        catch (const std::exception& ex)
        {
            std::string mess = ex.what();
            std::cout << "Error: " << mess << std::endl;
            throw ex;
        }
    });

    server_.Delete(versionPrefix_ + "/simulations", [&](const httplib::Request& req, httplib::Response& res) {

    });

    std::cout << "HTTP server is listening on port " << port_ << std::endl;
    server_.listen("localhost", port_);
}

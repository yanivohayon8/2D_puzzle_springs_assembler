#include "http_server.h"

//#include "Puzzle.h"

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
    dataLoader_.loadVertexMatings(*puzzle_.getGroundTruthMatings(), "springs_anchors_correct.csv");
    dataLoader_.loadPieces(*puzzle_.getPieces());

    res.status = 200;
    res.set_content("Puzzle Loaded", "text/plain");
}


void HTTPServer::run(int &i)
{
    httplib::Server server_;
    server_.Get(versionPrefix_+"/sanity", [&](const httplib::Request& req, httplib::Response& res) {

        std::cout << "Received request with params:" << std::endl;
        for (const auto& param : req.params) {
            std::cout << param.first << ": " << param.second << std::endl;
        }

        // Echo the body of the request
        res.status = 200;
        res.set_content("Hello World", "text/plain");
    });

    int j = 10;
    std::string puzzleDir;
    /*server_.Post(versionPrefix_+"/simulations", [&j,&puzzleDir](const httplib::Request& req, httplib::Response& res) {

        std::cout << j++ << std::endl;
        handlePuzzleLoading(req, res, puzzleDir);
    });*/

    server_.Post(versionPrefix_ + "/simulations", [&](const httplib::Request & req, httplib::Response & res) {
        handlePuzzleLoading(req, res);
    });

    std::cout << "HTTP server is listening on port " << port_ << std::endl;
    server_.listen("localhost", port_);
}

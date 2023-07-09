#include "http_server.h"
#include "cpp-httplib/httplib.h"
#include "Puzzle.h"

HTTPServer::HTTPServer(int port)
    : port_(port)
{
}

void handlePuzzleLoading(const httplib::Request& req, httplib::Response& res, std::string puzzleDirectory)
{
    std::vector<std::string> requestedParams = { "dataset","puzzle" };

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
        std::string reqPuzzleName = req.get_param_value("puzzle");
        std::string delimiter = "-";

        size_t pos = 0;
        size_t lastPos = 0;
        std::string directory;
        std::string puzzleNum;
        std::string noise;

        try
        {
            std::vector<std::string> result;
            std::string delimiter = "-";
            std::size_t pos = 0;
            std::size_t found;

            while ((found = reqPuzzleName.find(delimiter, pos)) != std::string::npos) {
                std::string token = reqPuzzleName.substr(pos, found - pos);
                result.push_back(token);
                pos = found + delimiter.length();
            }

            std::string lastToken = reqPuzzleName.substr(pos);
            result.push_back(lastToken);

            if (result.size() != 3) {
                throw std::runtime_error("Invalid input: Three dashes are required.");
            }

            directory = result[0];
            puzzleNum = result[1];
            noise = result[2];
        }
        catch (const std::exception& e) {
            res.status = 400;
            std::string mess = e.what();
            res.set_content("For the puzzle parameter: " + mess, "text/plain");
            return;
        }

        //std::string puzzleDirectory = "../data/ofir/" + directory + "/" + puzzleNum + "/" + noise;
        //std::cout << "Recieved simulation for puzzle in " << puzzleDirectory << std::endl;
        puzzleDirectory = "../data/ofir/" + directory + "/" + puzzleNum + "/" + noise;
    }

    std::cout << "Recieved simulation for puzzle in " << puzzleDirectory << std::endl;

    res.status = 200;
    res.set_content("<simulation id>", "text/plain");
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
    server_.Post(versionPrefix_+"/simulations", [&j,&puzzleDir](const httplib::Request& req, httplib::Response& res) {

        std::cout << j++ << std::endl;
        //handlePuzzleLoading(req, res, puzzleDir);
        //stam();
    });

    std::cout << "HTTP server is listening on port " << port_ << std::endl;
    server_.listen("localhost", port_);
}

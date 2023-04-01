#include "DataLoader.h"

struct Row {
	int index;
	double x;
	double y;
};


DataLoader::DataLoader(std::string puzzleDirectoryPath)
{
    puzzleDirectoryPath_ = puzzleDirectoryPath;
}

void DataLoader::loadPieces(std::vector<Piece>& olstPiece)
{
    std::string piecesFile = puzzleDirectoryPath_ + "/pieces.csv";
    std::ifstream infile(piecesFile);

    if (!infile.is_open()) {
        std::cerr << "Failed to open file: " << piecesFile << std::endl;
        return;
    }

    std::string line;
    int pieceId;
    double x, y;
    char comma;  // represent ',' in file, ignored.
    std::getline(infile, line);//skip the first line because its a header

    while (std::getline(infile, line)) {
        std::istringstream iss(line);

        if (!(iss >> pieceId >> comma >> x >> comma >> y)) {
            std::cerr << "Failed to read line: " << line << std::endl;
            continue;
        }

        std::cout << pieceId << ", " << x << ", " << y << std::endl;
    }
}
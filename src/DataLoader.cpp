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
    /*
        Loads the computed pieces. The csv headers are the following: piece,x,y.
        Unfortunately, the piece ids were saved in double form so we convert them into int.
    */

    std::string piecesFile = puzzleDirectoryPath_ + "/pieces.csv";
    std::ifstream infile(piecesFile);

    if (!infile.is_open()) {
        std::cerr << "Failed to open file: " << piecesFile << std::endl;
        return;
    }

    typedef Eigen::Matrix<double, Eigen::Dynamic, 2, Eigen::RowMajor> MatrixX2D_r;
    std::string line;
    double pieceId;
    double x, y;
    char comma;  // represent ',' in file, ignored.
    std::vector<std::pair<double,double>> coords;
    int currPieceId = -1;
    std::getline(infile, line);//skip the first line because it is a header


    while (std::getline(infile, line)) {
        std::istringstream iss(line);

        if (!(iss >> pieceId >> comma >> x >> comma >> y)) {
            throw "Failed to read line: " + line;
            continue;
        }

        if (currPieceId==-1)
        {
            currPieceId = pieceId;
        }

        if (currPieceId!=pieceId)
        {
            //auto data_debug= coords.data();
            Eigen::MatrixXd rtData = Eigen::Map<MatrixX2D_r const>(&(coords[0].first), coords.size(), 2).cast<double>();
            rtData = rtData.rowwise() - rtData.colwise().mean(); 
            Piece newPiece = Piece(int(currPieceId), rtData);
            olstPiece.push_back(newPiece);
            coords.clear();
        }

        std::pair<double, double> coord;
        coord.first = x;
        coord.second = y;
        coords.push_back(coord);
        currPieceId = pieceId;
        //std::cout << pieceId << ", " << x << ", " << y << std::endl;
    }

    Eigen::MatrixXd rtData = Eigen::Map<MatrixX2D_r const>(&(coords[0].first), coords.size(), 2).cast<double>();
    rtData = rtData.rowwise() - rtData.colwise().mean();
    Piece newPiece = Piece(currPieceId, rtData);
    olstPiece.push_back(newPiece);
}

void DataLoader::loadEdgeMatings(std::vector<EdgeMating>& olstMatings)
{
    /*
        Loads the matings between the edges. The csv headers are piece1,edge1,piece2,edge2. All of them are int.
    */
    std::string piecesFile = puzzleDirectoryPath_ + "/ground_truth_rels.csv";
    std::ifstream infile(piecesFile);

    if (!infile.is_open()) {
        std::cerr << "Failed to open file: " << piecesFile << std::endl;
        return;
    }

    std::string line;
    int firstPieceId, secondPieceId, firstPieceEdge, secondPieceEdge;
    char comma;  // represent ',' in file, ignored.
    std::getline(infile, line);//skip the first line because it is a header


    while (std::getline(infile, line)) {
        std::istringstream iss(line);

        if (!(iss >> firstPieceId >> comma >> firstPieceEdge >> comma >> secondPieceId >> comma >> secondPieceEdge)) {
            throw "Failed to read line: " + line;
            continue;
        }

        EdgeMating mating(firstPieceId, firstPieceEdge, secondPieceId, secondPieceEdge);
        olstMatings.push_back(mating);
    }
}
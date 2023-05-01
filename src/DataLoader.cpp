#include "DataLoader.h"

struct Row {
	int index;
	double x;
	double y;
};

typedef Eigen::Matrix<double, Eigen::Dynamic, 2, Eigen::RowMajor> MatrixX2D_r;

DataLoader::DataLoader(std::string puzzleDirectoryPath)
{
    puzzleDirectoryPath_ = puzzleDirectoryPath;
}

void flipAndCenterPolygon(Eigen::MatrixXd& coords)
{
    // Compute center of mass
    Eigen::Vector2d centerOfMass = coords.colwise().mean();

    // Flip coordinates using mirror transformation
    Eigen::MatrixXd flippedCoords = coords;
    coords.col(1) = centerOfMass.col(1) - (coords.col(1) - centerOfMass.col(1));

    // Center coordinates with respect to the center of mass
    coords.rowwise() -= centerOfMass.transpose();
}

void DataLoader::coordsToEigenCoords(Eigen::MatrixXd& eigenCoords, std::vector<std::pair<double, double>>& coords)
{
    eigenCoords = Eigen::Map<MatrixX2D_r const>(&(coords[0].first), coords.size(), 2).cast<double>();
    eigenCoords = eigenCoords.rowwise() - eigenCoords.colwise().mean(); // put as comment for repair
}

void DataLoader::loadPieces(std::vector<Piece>& olstPiece, bool isOfir)
{
    /*
        Loads the computed pieces. The csv headers are the following: piece,x,y.
        Unfortunately, the piece ids were saved in double form so we convert them into int.
    */

    std::string piecesFile = puzzleDirectoryPath_ + "/pieces.csv";
    //std::string piecesFile = puzzleDirectoryPath_ + "/ground_truth_puzzle.csv";
    std::ifstream infile(piecesFile);

    if (!infile.is_open()) {
        std::cerr << "Failed to open file: " << piecesFile << std::endl;
        return;
    }

    
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
            Eigen::MatrixXd rtData;
            coordsToEigenCoords(rtData, coords);

            Piece newPiece = Piece(int(currPieceId), rtData);
            olstPiece.push_back(newPiece);
            coords.clear();
        }

        std::pair<double, double> coord;

        if (isOfir)
        {
            // Hardcoded to ofir puzzle types
            x /= 1000; // The scale shouldbe 0.x...
            y /= 1000; // because of the input is in hundreds..
        }

        coord.first = x;
        coord.second = y;
        coords.push_back(coord);
        currPieceId = pieceId;
        //std::cout << pieceId << ", " << x << ", " << y << std::endl;
    }

    Eigen::MatrixXd rtData;
    coordsToEigenCoords(rtData, coords);

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

void DataLoader::loadVertexMatings(std::vector<VertexMating>& olstMatings)
{
    /*
        Loads the matings between the edges. The csv headers are piece1,vertex1,piece2,vertex2. All of them are int.
    */
    std::string piecesFile = puzzleDirectoryPath_ + "/ground_truth_rels_vertex.csv";
    std::ifstream infile(piecesFile);

    if (!infile.is_open()) {
        std::cerr << "Failed to open file: " << piecesFile << std::endl;
        return;
    }

    std::string line;
    int firstPieceId, secondPieceId, firstPieceVertex, secondPieceVertex;
    char comma;  // represent ',' in file, ignored.
    std::getline(infile, line);//skip the first line because it is a header


    while (std::getline(infile, line)) {
        std::istringstream iss(line);

        if (!(iss >> firstPieceId >> comma >> firstPieceVertex >> comma >> secondPieceId >> comma >> secondPieceVertex)) {
            throw "Failed to read line: " + line;
            continue;
        }

        VertexMating mating(firstPieceId, firstPieceVertex, secondPieceId, secondPieceVertex);
        olstMatings.push_back(mating);
    }
}

void DataLoader::loadExtraInfo(std::vector<Piece>& olstPiece)
{
    std::string piecesFile = puzzleDirectoryPath_ + "/info.csv";
    std::ifstream infile(piecesFile);

    if (!infile.is_open()) {
        std::cerr << "Failed to open file: " << piecesFile << std::endl;
        return;
    }

    std::string line;
    int pieceId, is_rotated;
    char comma;  // represent ',' in file, ignored.
    std::getline(infile, line);//skip the first line because it is a header


    while (std::getline(infile, line)) {
        std::istringstream iss(line);

        if (!(iss >> pieceId >> comma >> is_rotated)) {
            throw "Failed to read line: " + line;
            continue;
        }

        for (auto& piece:olstPiece)
        {
            if (piece.id_ == pieceId)
            {
                piece.isRotationFixed = bool(is_rotated);
            }
        }
    }
}
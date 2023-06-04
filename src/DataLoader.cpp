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
    //eigenCoords = eigenCoords.rowwise() - eigenCoords.colwise().mean(); // put as comment for repair
}

std::string DataLoader::getImagePath(std::string pieceId)
{
    return puzzleDirectoryPath_ + "/images/" + pieceId + ".png";
}

void DataLoader::loadPieces(std::vector<Piece>& olstPiece,bool isOfir)
{
    /*
        Loads the computed pieces. The csv headers are the following: piece,x,y.
        Unfortunately, the piece ids were saved in double form so we convert them into int.
    */

    //std::string piecesFile = pieces.csv"; //puzzleDirectoryPath_ + "/pieces.csv"; //"C:\\Users\\97254pieces.csv"; //puzzleDirectoryPath_ + "/pieces.csv"; //"C:\\Users\\97254\\Desktop\\msc\\RePAIR\\projects\\springs_assembler_sfml\\2D_puzzle_springs_assembler\\data\\ofir\\RePAIR\\group_39\\pieces.csv"; //puzzleDirectoryPath_ + "/pieces.csv";// + pieces_file_name;
    std::string piecesFile = puzzleDirectoryPath_ + "/pieces.csv";// + pieces_file_name;
    
    std::ifstream infile(piecesFile);

    if (!infile.is_open()) {
        std::cerr << "Failed to open file: " << piecesFile << std::endl;
        return;
    }

    
    std::string line;
    std::string pieceId;
    double x, y;
    std::string xStr, yStr;
    std::vector<std::pair<double,double>> coords;
    std::getline(infile, line);//skip the first line because it is a header
    std::string currPieceId = "";


    while (std::getline(infile, line)) {
        
        std::stringstream ss(line);

        std::getline(ss, pieceId, ',');
        std::getline(ss, xStr, ',');
        x = std::stod(xStr);
        std::getline(ss, yStr, ',');
        y = std::stod(yStr);

        if (currPieceId=="")
        {
            currPieceId = pieceId;
        }

        if (currPieceId!=pieceId)
        {
            //auto data_debug= coords.data();
            Eigen::MatrixXd rtData;
            coordsToEigenCoords(rtData, coords);

            Piece newPiece = Piece(currPieceId, rtData, getImagePath(currPieceId)); // Does all images are pngs? 
            olstPiece.push_back(newPiece);
            coords.clear();
        }

        std::pair<double, double> coord;

        if (isOfir)
        {
            // Hardcoded to ofir puzzle types
            //x /= 1000; // The scale shouldbe 0.x...
            //y /= 1000; // because of the input is in hundreds..
            x *= SCALE_IMAGE_COORDINATES_TO_BOX2D;
            y *= SCALE_IMAGE_COORDINATES_TO_BOX2D;
        }

        coord.first = x;
        coord.second = y;
        coords.push_back(coord);
        currPieceId = pieceId;
    }

    Eigen::MatrixXd rtData;
    coordsToEigenCoords(rtData, coords);

    Piece newPiece = Piece(currPieceId, rtData, getImagePath(currPieceId));
    olstPiece.push_back(newPiece);
}

void DataLoader::loadCoordinates_(std::string fileName, bool isOfir )
{

}

void DataLoader::loadVertexMatings(std::vector<VertexMating>& olstMatings, std::string fileName)
{
    /*
        Loads the matings between the edges. The csv headers are piece1,vertex1,piece2,vertex2. All of them are int.
    */
    std::string piecesFile = puzzleDirectoryPath_ + "/" + fileName;
    std::ifstream infile(piecesFile);

    if (!infile.is_open()) {
        std::cerr << "Failed to open file: " << piecesFile << std::endl;
        return;
    }

    std::string line;
    //int firstPieceId, secondPieceId, firstPieceVertex, secondPieceVertex;
    int firstPieceVertex, secondPieceVertex;
    std::string firstPieceVertexStr, secondPieceVertexStr;
    std::string firstPieceId, secondPieceId;
    char comma;  // represent ',' in file, ignored.
    std::getline(infile, line);//skip the first line because it is a header


    while (std::getline(infile, line)) {
        std::stringstream ss(line);
        std::getline(ss, firstPieceId, ',');
        std::getline(ss, firstPieceVertexStr, ',');
        firstPieceVertex = std::stoi(firstPieceVertexStr);
        std::getline(ss, secondPieceId, ',');
        std::getline(ss, secondPieceVertexStr, ',');
        secondPieceVertex = std::stoi(secondPieceVertexStr);

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
    //int pieceId, is_rotated;
    int is_rotated;
    std::string pieceId;

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
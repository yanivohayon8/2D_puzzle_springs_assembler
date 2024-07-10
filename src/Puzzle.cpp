//#include "Puzzle.h"
//
//Puzzle::Puzzle()
//{
//}
//
//Puzzle::Puzzle(std::vector<Piece> &pieces)
//{
//	pieces_ = pieces;
//	//groundTruthMatings_ = groundTruthMatings;
//
//	for (auto& piece : pieces_)
//	{
//		id2piece_.insert({ piece.id_,piece });
//	}
//}
//
////std::vector<VertexMating>* Puzzle::getGroundTruthMatings()
////{
////	return &groundTruthMatings_;
////}
////void Puzzle::setGroundTruthMatings(std::vector<VertexMating>& newGroundTruthMatings)
////{
////	groundTruthMatings_ = newGroundTruthMatings;
////}
//
//std::vector<Piece>* Puzzle::getPieces()
//{
//	return &pieces_;
//}
//
//void Puzzle::setPieces(std::vector<Piece>& newPieces)
//{
//	pieces_ = newPieces;
//
//	id2piece_.clear(); // Should we delete actively the pieces inside of it
//
//	for (auto& piece : pieces_)
//	{
//		id2piece_.insert({ piece.id_,piece });
//	}
//}
//
//
//void Puzzle::findPiecesToReconstruct(std::vector<Piece> &oReconstructPieces,std::vector<VertexMating*>& matings)
//{
//	//oReconstructPieces.clear();
//	std::vector<std::string> reconstructPiecesIds;
//
//	for (auto& mating : matings)
//	{
//		if (!std::count(reconstructPiecesIds.begin(), reconstructPiecesIds.end(), mating->firstPieceId_))
//		{
//			Piece& p1 = id2piece_.at(mating->firstPieceId_);
//			oReconstructPieces.push_back(p1);
//			reconstructPiecesIds.push_back(mating->firstPieceId_);
//		}
//
//		if (!std::count(reconstructPiecesIds.begin(), reconstructPiecesIds.end(), mating->secondPieceId_))
//		{
//			Piece& p2 = id2piece_.at(mating->secondPieceId_);
//			oReconstructPieces.push_back(p2);
//			reconstructPiecesIds.push_back(mating->secondPieceId_);
//		}
//	}	
//}
//
//

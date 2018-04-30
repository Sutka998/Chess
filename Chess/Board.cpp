#include "Board.h"

namespace ch {

	Board::Board()
		: m_whiteCount(0), m_blackCount(0)
	{
		for (int i = 0; i < 64; i++) {
			m_pieceArray[i] = nullptr;
		}
	}

	void Board::deletePieceAt(const Position& pos) {
		Piece*& piecPtr = m_at(pos);
		try{
			if(piecPtr != nullptr) {
				std::vector<Piece*>& piecesV = (piecPtr->getColor() == Color::WHITE)? m_whitePieces : m_blackPieces;
				for(auto it = piecesV.begin(); it != piecesV.end(); it++) {
					if((*it) == piecPtr) {//The piece's pointer found in the piece list
						piecesV.erase(it);
						break;
					}
					throw std::logic_error("The piece that should be deleted is not in the vector Board::deletePieceAt");
				}
			}
		}
		catch(...){
			delete piecPtr;
			piecPtr = nullptr;
			throw;
		}
		delete piecPtr;
		piecPtr = nullptr;
	}

	void Board::movePiece(const Position& from, const Position& dest) {
		Piece*& destPtr = m_at(dest);
		if(destPtr == nullptr) {
			Piece* fromPtr = m_at(from);
			destPtr = fromPtr;
			fromPtr = nullptr;
			return;
		}
		throw std::logic_error("Destination is not nullptr.");
	}

	Board::~Board() {
		try {
			for (int i = 0; i < 64 ; i++) {
				delete m_pieceArray[i];
			}
		}
		catch(...) {

		}
	}

}
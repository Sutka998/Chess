#include "Board.h"

namespace ch {

	Board::Board()
	{
		for (int i = 0; i < 64; i++) {
			m_pieceArray[i] = nullptr;
		}
	}

	void Board::save() {
		for (int i = 0; i < m_deleteList.size(); i++) {
			delete m_deleteList[i]; //deleting delete list pointers
		}
		for (int i = 0; i < 64; i++) {
			m_savedArray[i] = m_pieceArray[i]; //Saving the data
		}
	}

	void Board::undo() {
		for (int i = 0; i < m_deleteList.size(); i++) {
			//Putting back the piece to the Piece list
			((m_deleteList[i]->getColor() == Color::WHITE) ? m_whitePieces : m_blackPieces).push_back(m_deleteList[i]);
		}
		m_deleteList.clear();
		for (int i = 0; i < 64; i++) {
			m_pieceArray[i] = m_savedArray[i]; //Loading back the data
		}
	}

	void Board::deletePieceAt(const Position& pos) {
		Piece*& piecPtr = m_at(pos);
			if(piecPtr != nullptr) {
				std::vector<Piece*>& piecesV = (piecPtr->getColor() == Color::WHITE)? m_whitePieces : m_blackPieces;
				for(auto it = piecesV.begin(); it != piecesV.end(); it++) {
					if((*it) == piecPtr) {//The piece's pointer found in the piece list
						piecesV.erase(it);
						m_deleteList.push_back(piecPtr);
						break;
					}
					throw std::logic_error("The piece that should be deleted is not in the vector Board::deletePieceAt");
				}
			}

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
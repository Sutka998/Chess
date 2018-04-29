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
		Piece* piecPos = m_at(pos);
		delete piecPos;
		piecPos = nullptr;
	}

	void Board::movePiece(const Position& from, const Position& dest) {
		Piece* destPtr = m_at(dest);
		if(destPtr == nullptr) {
			Piece* fromPtr = m_at(from);
			destPtr = fromPtr;
			fromPtr = nullptr;
			return;
		}
		throw std::logic_error("Destination is not nullptr.");
	}

	const ch::Piece* Board::getPieceAt(const Position& pos) const {
		return m_at(pos);
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
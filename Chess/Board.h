#pragma once
#include "Piece.h"

namespace ch {
	class Board {
		//0: A1, 1: A2 2: A3 ... 8: B1 ... 63: H8
		Piece* m_pieceArray[64];
		unsigned short m_whiteCount;
		unsigned short m_blackCount;
	public:
		Board() {};

		bool placePieceAt(const Position&, PieceName);
		bool deletePieceAt(const Position&);
		const Piece& getPieceAt(const Position&) const;
		
	};
}
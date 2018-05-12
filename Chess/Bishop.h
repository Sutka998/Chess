#pragma once
#include "Piece.h"

namespace ch {

	class Bishop: public Piece
	{
		void m_evalHitGrid() {}
		void m_evalMoveGrid() {}
	public:
		Bishop(Color col, const Position& pos) : Piece(col, pos, PieceType::BISHOP) {}
		~Bishop(void);
	};
}

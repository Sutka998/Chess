#pragma once
#include "Piece.h"

namespace ch {
	class Rook : public Piece
	{
		void m_evalHitGrid();
		void m_evalMoveGrid();
	public:
		Rook(Color col, const Position& pos) : Piece(col, pos, PieceType::ROOK) {
			m_evalMoveGrid();
			m_evalHitGrid();
		}
		~Rook(void){}
	};

}
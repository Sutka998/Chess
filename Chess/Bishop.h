#pragma once
#include "Piece.h"

namespace ch {

	class Bishop: public Piece
	{
		void m_evalHitGrid() {
			m_hGridEqMvGrid();
		}
		void m_evalMoveGrid();
	public:
		Bishop(Color col, const Position& pos) : Piece(col, pos, PieceType::BISHOP) {
			m_evalMoveGrid();
			m_evalHitGrid();
		}
		~Bishop(void){}
	};
}

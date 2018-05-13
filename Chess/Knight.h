#pragma once
#include "Piece.h"

namespace ch {
	class Knight : public Piece
	{
		void m_evalHitGrid();
		void m_evalMoveGrid();
	public:
		Knight(Color col, const Position& pos) : Piece(col, pos, PieceType::KNIGHT) {
			m_evalMoveGrid();
			m_evalHitGrid();
		}
		~Knight(void){	}
	};

}
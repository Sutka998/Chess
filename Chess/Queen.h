#pragma once
#include "Piece.h"
namespace ch {

	class Queen : public Piece
	{
		void m_evalHitGrid() {m_hGridEqMvGrid();}
		void m_evalMoveGrid();
	public:
		Queen(Color col, const Position& pos) : Piece(col, pos, PieceType::QUEEN) 
		{
			m_evalMoveGrid();
			m_evalHitGrid();
		}
		~Queen(void){}
	};

}
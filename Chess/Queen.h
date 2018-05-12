#pragma once
#include "Piece.h"
namespace ch {

	class Queen : public Piece
	{
		void m_evalHitGrid() {}
		void m_evalMoveGrid() {}
	public:
		Queen(Color col, const Position& pos) : Piece(col, pos, PieceType::QUEEN) {}
		~Queen(void){}
	};

}
#pragma once
#include "Piece.h"

namespace ch {

	class King: public Piece
	{
		void m_evalHitGrid() {}
		void m_evalMoveGrid() {}
	public:
		const static PieceType pieceType;
		King(Color col, const Position& pos) : Piece(col, pos, PieceType::KING){}
		~King(void);
	};

}
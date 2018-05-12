#pragma once
#include "Piece.h"

namespace ch {

	class King: public Piece
	{
		void m_evalHitGrid() {}
		void m_evalMoveGrid() {}
	public:
		King(Color col, const Position& pos) : Piece(col, pos) {}
		~King(void);
	};

}
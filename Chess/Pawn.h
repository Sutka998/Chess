#pragma once
#include "Piece.h"
namespace ch {

	class Pawn : public Piece {
	private:
		void m_evalMoveGrid();
		void m_evalHitGrid();
		//Adding the elements to the moving Grid, for the white and black pieces.
		void m_mGridAdd_white();
		void m_mGridAdd_black();
		//If the pawn can be removed by EnPassant, it is true
		bool m_canBeEnPass;
	public:
		Pawn(Color, const Position&);
		bool Pawn::Move_Hit(const Position&);
	};
}
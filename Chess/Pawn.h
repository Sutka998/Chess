#pragma once
#include "Piece.h"
namespace ch {

	class Pawn : public Piece {
	private:
		void m_evalMoveGrid();
		void m_evalHitGrid();
		//Adding the elements to the *MOVE Grid**, for the white and black pieces.
		void m_moveGridAdd_white();
		void m_moveGridAdd_black();
		//Adding the elements to the **HIT GRID**
		void m_hitGridAdd_white();
		void m_hitGridAdd_black();
	public:
		//If the pawn can be removed by EnPassant, it is true, should be reseted from the outside
		bool m_canBeEnPass;
		const static PieceType pieceType;
		Pawn(Color, const Position&);
		virtual void Move_Hit(const Position&);
		~Pawn(void){}
	};
}
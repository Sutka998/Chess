#pragma once
#include "position.h"
#include <vector>

namespace ch {
	enum class Color {
		BLACK, WHITE
	};
	enum class PieceType {
		PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING
	};
	enum class MovType{
		MOVE, HIT
	};

	class Piece {
	private:
		//Depends on the type of piece
		virtual void m_evalHitGrid() = 0;
		virtual void m_evalMoveGrid() = 0;
		bool m_isInGrid(MovType, const Position&) const;

		void m_move(const Position&);
		std::vector<Position> m_notCheckGrid;
	protected:
		//Have hitGrid, or moveGrid been evaluated since the last step
		bool m_mGridEvaled;
		bool m_hGridEvaled;

		bool m_isMoved;
		Position m_position;
		const Color m_color;

		mutable bool m_isCacheValid;
		mutable	Position m_canMoveHitCache;
		//Movements that are can be done
		//Contains the coordinates, where the piece can step, no repeatings.
		std::vector<Position> m_hitGrid;
		std::vector<Position> m_moveGrid;
	public:
		std::vector<Position>& noCheckGrid; //Finally the piece should step on these (check rule)
		bool noCheckG_isnotapplied; //When the noCheckGrid is empty, because there are no restriction (hit/move grid is valid)

		const static PieceType pieceType;
		Piece(Color, const Position&);

		bool canMoveHit(const Position&, MovType) const;
		virtual void Move_Hit(const Position&);

		//Getters
		const std::vector<Position>& getHitGrid() const {return m_hitGrid;}
		const std::vector<Position>& getMoveGrid() const {return m_moveGrid;}
		const Position& getPosition() const { return m_position;}
		bool isMoved() const {return m_isMoved;}
		Color getColor() const {return m_color;}
	};
}
#pragma once
#include "position.h"
#include <vector>

namespace ch {
	enum class Color {
		BLACK, WHITE
	};
	enum class PieceName {
		PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING
	};

	class Piece {
	private:
		//Depends on the type of piece
		virtual void m_evalHitGrid() = 0;
		virtual void m_evalMoveGrid() = 0;
	protected:
		//Have hitGrid, or moveGrid been evaluated since the last step
		bool m_mGridEvaled;
		bool m_hGridEvaled;

		bool m_isMoved;
		Position m_position;
		const Color m_color;
		//Movements that are can be done
		//Contains the coordinates, where the piece can step, no repeatings.
		std::vector<Position> m_hitGrid;
		std::vector<Position> m_moveGrid;
	public:
		const PieceName pieceType;
		Piece(Color, const Position&, PieceName);

		static bool isInGrid(const std::vector<Position>&, const Position&);
		//Getters
		virtual bool Move_Hit(const Position&);
		const std::vector<Position>& getHitGrid() const {return m_hitGrid;}
		const std::vector<Position>& getMoveGrid() const {return m_moveGrid;}
		const Position& getPosition() const { return m_position;}
		bool isMoved() const {return m_isMoved;}
		Color getColor() const {return m_color;}
	};
}
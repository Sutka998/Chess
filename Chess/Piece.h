#pragma once
#include "position.h"
#include <vector>

namespace ch {
	enum class Color {
		BLACK, WHITE
	};
	class Piece {
	private:
		bool m_isMoved;
		Position m_position;
		const Color m_color;
		std::vector<Position> m_hitGrid;
		std::vector<Position> m_moveGrid;
		
		virtual void m_evalHitGrid() = 0;
		virtual void m_evalMoveGrid() = 0;

	public:
		
		Piece(Color, const Position&);
		static bool isInGrid(const std::vector<Position>&, const Position&);

		bool Move(const Position&);
		bool Hit(const Position&);
		const std::vector<Position>& getHitGrid() const {return m_hitGrid;}
		const std::vector<Position>& getMoveGrid() const {return m_moveGrid;}

		bool isMoved() const {return m_isMoved;}
	};

}
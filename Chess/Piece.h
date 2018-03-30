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
		std::vector<Coords> m_hitGrid;
		std::vector<Coords> m_moveGrid;
		
		virtual void m_evalHitGrid() = 0;
		virtual void m_evalMoveGrid() = 0;

	public:
		
		Piece(Color color, const Coords& coords) : m_color(color), m_isMoved(false), m_position(coords) {}
		static bool isInGrid(const std::vector<Coords>&, const Position&);

		bool Move(const Position&);
		bool Hit(const Position&);
		const std::vector<Coords>& getHitGrid() const {return m_hitGrid;}
		const std::vector<Coords>& getMoveGrid() const {return m_moveGrid;}

		bool isMoved() const {return m_isMoved;}
		const Coords& getPosition() const {return m_position.getCoords();}

		virtual ~Piece();
	};

}
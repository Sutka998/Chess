#include "Piece.h"

namespace ch {
	
	Piece::Piece(Color color, const Position& position, PieceName pn) 
		: m_color(color), m_position(position), pieceType(pn),
		m_isMoved(false), m_mGridEvaled(false), m_hGridEvaled(false)
	{}

	bool Piece::isInGrid(const std::vector<Position>& m_grid, const Position& pos)
	{
		for (auto iter = m_grid.begin(); iter != m_grid.end(); iter++)	{ 
			if(*iter == pos) return true;
		}
		return false;
	}

	bool Piece::Move_Hit(const Position& pos) {
		if(isInGrid(m_moveGrid, pos) || isInGrid(m_hitGrid, pos)) {
			m_position = pos;
			m_evalHitGrid(); //Moved, refreshing accessible tiles.
			m_evalMoveGrid();
			return true;
		}
		return false;
	}
}
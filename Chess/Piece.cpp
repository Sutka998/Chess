#include "Piece.h"

namespace ch {
	
	bool Piece::isInGrid(const std::vector<Coords>& m_grid, const Position& pos)
	{
		for (auto iter = m_grid.begin(); iter != m_grid.end(); iter++)	{
			if(*iter == pos.getCoords()) return true;
		}
		return false;
	}

}
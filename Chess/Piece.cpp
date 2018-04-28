#include "Piece.h"

namespace ch {
	
	Piece::Piece(Color color, const Position& position, PieceName pn) 
		: m_color(color), m_position(position), pieceType(pn), 
		m_isMoved(false), m_mGridEvaled(false), m_hGridEvaled(false), m_isCacheValid(false)
	{}

	bool Piece::canMoveHit(const Position& pos, MovType mvtyp) const {
		//Is there any cached value
		if(m_isCacheValid && (pos == m_canMoveHitCache)){
			return true;
		}
		if(m_isInGrid(mvtyp, pos)) { //Then check for the grid elements
			m_canMoveHitCache = pos;
			m_isCacheValid = true;
			return true;
		}
		return false;
	} 

	bool Piece::m_isInGrid(MovType mvtype, const Position& pos) const {
		auto m_grid = &m_hitGrid;
		if(mvtype == MovType::MOVE)	{m_grid = &m_moveGrid;}

		for (auto iter = m_grid->begin(); iter != m_grid->end(); iter++)	{ 
			if(*iter == pos) return true;
		}
		return false;
	}

	void Piece::m_move(const Position& pos) {
		m_position = pos;
		m_mGridEvaled = m_hGridEvaled = false;//The grids no longer evaluated, it needs to be false, for refreshing the grids.
		m_evalHitGrid(); //Moved, refreshing the grids.
		m_evalMoveGrid();
		m_isCacheValid = false; //the cached moving position is no longer valid.
	}

	void Piece::Move_Hit(const Position& pos) {
		if(canMoveHit(pos, MovType::HIT) || canMoveHit(pos, MovType::MOVE)) {
			m_move(pos);
			return;
		}
		throw std::invalid_argument("Position is not in the grid. Unable to move the piece.");
	}
}
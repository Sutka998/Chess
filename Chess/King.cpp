#include "King.h"

namespace ch {

	void King::m_evalHitGrid() {
		if(m_hGridEvaled) {
			return;
		}
		m_hitGrid.clear();
		m_evalMoveGrid();
		m_hitGrid = m_moveGrid;
		m_hGridEvaled = true;
		return;
	}

	void King::m_evalMoveGrid() {
		if(m_mGridEvaled) {
			return;
		}
		m_moveGrid.clear();
		unsigned row = m_position.getRow();
		unsigned column = m_position.getColumn().number;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if(i != 0 || j != 0) { // when 0-0, shouldn't check
					if(row +i >= 1 && row +i <= 8 && column +j >= 1 && column+j <= 8) {
						m_moveGrid.push_back(Position(Column(column + j), row + i));
					}
				}
			}
		}
		m_mGridEvaled = true;
		return;
	}

	const PieceType King::pieceType = PieceType::KING;
}
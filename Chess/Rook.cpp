#include "Rook.h"

namespace ch {

	void Rook::m_evalHitGrid() {
		if(m_hGridEvaled) {
			return;
		}
		m_hitGrid.clear();
		m_evalMoveGrid();
		m_hitGrid = m_moveGrid;
		m_hGridEvaled = true;
		return;
	}

	void Rook::m_evalMoveGrid() {
		if(m_mGridEvaled) {
			return;
		}
		m_moveGrid.clear();
		unsigned row = m_position.getRow();
		unsigned column = m_position.getColumn().number;
		for (int i = 1; i <= 8 ; i++) {
			m_moveGrid.push_back(Position(Column(column), i)); //Pushing fields from the current column
			m_moveGrid.push_back(Position(Column(i), row )); //Pushing fields from the current row
		}
		m_mGridEvaled = true;
		return;
	}

}
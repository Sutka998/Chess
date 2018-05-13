#include "Queen.h"
namespace ch {

	void Queen::m_evalMoveGrid() {
		if(m_mGridEvaled) {
			return;
		}
		m_moveGrid.clear();
		unsigned row = m_position.getRow();
		unsigned column = m_position.getColumn().number;
		//FROM ROOK
		for (int i = 1; i <= 8 ; i++) {
			m_moveGrid.push_back(Position(Column(column), i)); //Pushing fields from the current column
			m_moveGrid.push_back(Position(Column(i), row )); //Pushing fields from the current row
		}
		//FROM BISHOP
		for (int i = 1; i <= 8; i++) {
			int y1 = i - column + row;
			int y2 = - i + column + row;

			if(y1 >= 1 && y1 <= 8) {
				m_moveGrid.push_back(Position(Column(i), y1));//The increasing line
			}
			if (y2>= 1 && y2 <= 8) {
				m_moveGrid.push_back(Position(Column(i), y2));//The decreasing line
			}
		}

		m_mGridEvaled = true;
		return;
	}

}
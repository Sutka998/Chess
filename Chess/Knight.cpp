#include "Knight.h"

namespace ch {

	void Knight::m_evalMoveGrid() {
		if(m_mGridEvaled) {
			return;
		}
		m_moveGrid.clear();
		unsigned row = m_position.getRow();
		unsigned column = m_position.getColumn().number;
		int nums[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}}; //Array for easier L shape adding
		for (int i = 0; i < 8; i++) {
			//Checking, if the position is not overrun
			if(row +nums[i][0] >= 1 && row +nums[i][0] <= 8 && column +nums[i][1] >= 1 && column+nums[i][1] <= 8) {
				m_moveGrid.push_back(Position(Column(column + nums[i][1]), row + nums[i][0])); //Then add the valid ones.
			}
		}
		m_mGridEvaled = true;
		return;
	}

}
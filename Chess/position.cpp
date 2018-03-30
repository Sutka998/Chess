#include "position.h"
#include <stdexcept>

namespace ch {
	Position::Position (Column column, unsigned short row) : m_column(column) {
		setRow(row);
	}

	void Position::setRow(unsigned short row) {
		if(row >= 1 && row <= 8) {
			m_row = row;
		}
		throw std::invalid_argument("Row should be between 1 and 8");
	}

	void Position::setPosition(Column column, unsigned short row) {
		setColumn(column);
		setRow(row);
	}
}
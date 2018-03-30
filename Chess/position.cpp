#include "position.h"
#include <stdexcept>

namespace ch {
	
	Position::Position (const Coords& coords) : m_coords(coords) {
		setRow(coords.row);
	}

	void Position::setRow(unsigned short row) {
		if(row >= 1 && row <= 8) {
			m_coords.row = row;
		}
		throw std::invalid_argument("The row should be between 1 and 8");
	}

	void Position::setPosition(Coords c) {
		setColumn(c.column);
		setRow(c.row);
	}

	bool Position::operator==(const Position& that) const{
		return this->getCoords() == that.getCoords();
	}

	bool Coords::operator==(const Coords& that) const {
		return (this->column == that.column) && (this->row == that.row);
	}

}
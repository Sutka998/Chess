#include "Position.h"
#include <stdexcept>

namespace ch {
	
	Position::Position (Column c, unsigned short row) 
		: m_column(c) {
		setRow(row); //Row should be validated. Should be between 1 and 8.
	}

	void Position::setRow(unsigned short row) {
		if(row >= 1 && row <= 8) {
			m_row = row;
		}
		throw std::invalid_argument("The row should be between 1 and 8");
	}

	void Position::setPosition(Column column, unsigned short row) {
		m_column = column;
		setRow(row);
	}

	bool Position::operator==(const Position& that) const{
		return
			(this->getColumn() == that.getColumn())
			&&
			(this->getRow()) == that.getRow();
	}



}
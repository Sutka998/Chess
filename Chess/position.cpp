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
			return;
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
	//////////////////////////////////////////////////////////////////////////
	void Column::set(CL cl) {
		m_letter = cl;
		m_number = static_cast<unsigned short>(cl);
	}
	void Column::set(unsigned short number) {
		if(number <= 8 && number >= 1) {
			m_number = number;
			m_letter = static_cast<CL>(number);
			return;
		}
		throw std::invalid_argument("Column number should be between 1 and 8.");
	}

	Column Column::operator-(unsigned short num) const {
		Column c(*this);
		c.set(c.m_number-num);
		return c;
	}

	Column Column::operator+(unsigned short num) const {
		Column c(*this);
		c.set(c.m_number+num);
		return c;
	}

	bool Column::operator==(const Column& that) const{
		return this->m_number == that.m_number;
	}

	const Column& Column::operator=(const Column& that){
		this->m_letter = that.m_letter;
		this->m_number = that.m_number;
		return *this;
	}
}
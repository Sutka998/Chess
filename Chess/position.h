#pragma once

namespace ch {

	enum class Column {
		A = 1, B, C, D, E, F, G, H
	};

	class Position {
	private:
		unsigned short m_row;
		Column m_column;;
	public:
		Position (Column c = Column::A, unsigned short row = 2);

		void setRow(unsigned short);
		void setColumn(Column column) { m_column = column;}
		void setPosition(Column, unsigned short);

		unsigned short getRow() const {return m_row;}
		Column getColumn() const {return m_column;}

		bool operator==(const Position& that) const;
	};
}
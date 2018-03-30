#pragma once

namespace ch {
	class Position {
	private:
		unsigned short m_row;
		Column m_column;
	public:
		Position (Column column = Column::A, unsigned short row = 2);
		void setRow(unsigned short row);
		void setColumn(Column column) { m_column = column;}
		void setPosition(Column column, unsigned short row);

		Column getColumn() const {return m_column;}
		unsigned short getRow() const {return m_row;}
	};

	enum class Column {
		A = 1, B, C, D, E, F, G
	};
}
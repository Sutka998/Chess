#pragma once

namespace ch {

	enum class Column {
		A = 1, B, C, D, E, F, G, H
	};

	struct Coords {
		unsigned short row;
		Column column;
		Coords(Column c, unsigned short row = 2) : column(c), row(row) {}
		
		bool operator==(const Coords& that) const;
	};

	class Position {
	private:
		Coords m_coords;
	public:
		Position (const Coords&);
		void setRow(unsigned short);
		void setColumn(Column column) { m_coords.column = column;}
		void setPosition(Coords);

		Column getColumn() const {return m_coords.column;}
		unsigned short getRow() const {return m_coords.row;}
		const Coords& getCoords() const {return m_coords; }

		bool operator==(const Position& that) const;
	};
}
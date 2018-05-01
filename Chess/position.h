#pragma once
#include <vector>

namespace ch {

	class Column {
	public:
		enum class CL {
			A = 1, B, C, D, E, F, G, H
		};
	private:
		CL m_letter;
		unsigned short m_number;
	public:
		Column(CL letter) 
			: m_letter(letter), m_number(static_cast<unsigned short>(letter)),
			letter(m_letter), number(m_number)
		{}
		Column(unsigned short number) : letter(m_letter), number(m_number)
		{ set(number); }
		const CL& letter;
		const unsigned short& number;

		void set(CL);
		void set(unsigned short);

		const Column& operator=(const Column&);
		bool operator==(const Column&) const;
		Column operator+(unsigned short) const;
		Column operator-(unsigned short) const;
	};

	class Position {
	private:
		unsigned short m_row;
		Column m_column;;
	public:
		Position (Column c = Column::CL::A, unsigned short row = 2);
		//Setters
		void setRow(unsigned short);
		void setColumn(Column column) { m_column = column;}
		void setPosition(Column, unsigned short);
		//Getters
		unsigned short getRow() const {return m_row;}
		Column getColumn() const {return m_column;}
		//Operators
		bool operator==(const Position& that) const;
	};

	//Returns the intersection of two vectors
	template<class T>
	void vectorIntSect(const std::vector<T>& inA, const std::vector<T>& inB, std::vector<T>& out) {
		for (auto iterA = inA.begin(); iterA != inA.end(); iterA++) {
			for (auto iterB = inB.begin(); iterB != inB.end(); iterB++) {
				if((*iterA) == (*iterB)) {
					out.push_back(*iterA);
					break;
				}
			}
		}
	}
}
#pragma once
#include <vector>

namespace ch {
	/**
	* \file Position.h
	* \brief Contains the Column, and Position classes.
	*/

	class Column {
	public:
		enum class CL { /**< \brief The columns of the chess board */
			A = 1, B, C, D, E, F, G, H
		};
	private:
		CL m_letter;
		unsigned short m_number;
	public:
		/** \brief Creates a column, by a given letter
		*/
		Column(CL letter) 
			: m_letter(letter), m_number(static_cast<unsigned short>(letter)),
			letter(m_letter), number(m_number)
		{}
		/** \brief Creates a column, by a given number, should be between 1 and 8.
		*/
		Column(unsigned short number) : letter(m_letter), number(m_number)
		{ set(number); }
		/** \brief Copy constructor.
		*/
		Column(const Column& col) : letter(m_letter), number(m_number) {
			*this = col;
		}
		const CL& letter; /**< \brief  Reference to the letter format*/
		const unsigned short& number;  /**< \brief Reference to the number format */

		/** \brief Set the column, by a letter.
		*\param [in] CL enum class letter
		*/
		void set(CL);
		/** \brief Set the column by a number, between 1 and 8.
		*/
		void set(unsigned short);

		const Column& operator=(const Column&);
		bool operator==(const Column&) const;
		Column operator+(unsigned short) const;
		Column operator-(unsigned short) const;
	};

	class Position {
	private:
		unsigned short m_row;
		Column m_column;
	public:
		/** \brief Creates a position by a Column, and a number, should be between 1 and 8.
		*/
		Position (Column c = Column::CL::A, unsigned short row = 2);
		//Setters
		/** \brief Sets the row. Should be between 1-8, else exception is thrown.
		*/
		void setRow(unsigned short);
		/** \brief Sets the column.
		*/
		void setColumn(Column column) { m_column = column;}
		/** \brief Sets the whole position.
		*/
		void setPosition(Column, unsigned short);
		//Getters
		/** \brief Returns the current row.
		*/
		unsigned short getRow() const {return m_row;}
		/** \brief Returns the current column.
		*/
		Column getColumn() const {return m_column;}
		//Operators
		bool operator==(const Position& that) const;
	};

	/** \brief Returns the intersection of two vectors.
		*\param [in] vector a
		*\param [in] vector b
		*\param [out] the intersection of the two vectors.
		*/
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
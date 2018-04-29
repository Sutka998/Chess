#pragma once
#include "Piece.h"
#include <vector>

namespace ch {
	class Board {
		//0: A1, 1: A2 2: A3 ... 8: B1 ... 63: H8
		Piece* m_pieceArray[64];
		unsigned short m_whiteCount;
		unsigned short m_blackCount;
		Piece* m_at(const Position& pos) const{
			return m_pieceArray[pos.getRow()*8 + pos.getColumn().number];}//0th row, 0th column is the beginning

		Board(const Board&);
	public:
		Board();

		template<class PIECE_T>
		void placePieceAt(Color color, const Position& pos) {
			Piece* posPtr = m_at(pos);
			if(posPtr == nullptr) {
				try {
					posPtr = new PIECE_T(color, pos);
				}
				catch(...) {
					delete posPtr;
					posPtr = nullptr;
					throw;
				}
				if (color == Color::BLACK){m_blackCount++;	}
				else{m_whiteCount++;}
				return;
			}
			throw std::logic_error("Destination is not nullptr.");
		}

		void deletePieceAt(const Position&);

		void movePiece(const Position& from, const Position& dest);
		const Piece* getPieceAt(const Position&) const;
		
		~Board();
	};
}
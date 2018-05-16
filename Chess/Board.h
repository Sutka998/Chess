#pragma once
#include "Piece.h"
#include "PiecesH.h"
#include <vector>
#include <string>

namespace ch {
	class Board : public ISerializable{
		//0: A1, 1: A2 2: A3 ... 8: B1 ... 63: H8
		Piece* m_pieceArray[64];
		Piece* m_savedArray[64];
		Piece*& m_at(const Position& pos) {
			return m_pieceArray[(pos.getRow()-1)*8 + (pos.getColumn().number -1)];//0th row, 0th column is the beginning
 		}
		std::vector<Piece*> m_whitePieces;
		std::vector<Piece*> m_blackPieces;

		std::vector<Piece*> m_deleteList;

		Board(const Board&);
	public:
		Board();

		std::vector<ch::Piece*, std::allocator<ch::Piece*>>::iterator it_Begin(Color color) {
			return color == Color::WHITE ? m_whitePieces.begin() : m_blackPieces.begin();
		}
		std::vector<ch::Piece*, std::allocator<ch::Piece*>>::iterator it_End(Color color) {
			return color == Color::WHITE ? m_whitePieces.end() : m_blackPieces.end();
		}

		void save();
		void undo();

		void Serialize(std::ofstream&);
		void Deserialize(std::ifstream&);

		void placePieceAt(Color color, const Position& pos, PieceType pieceType);

		void deletePieceAt(const Position&);

		void movePiece(const Position& from, const Position& dest);
		Piece* getPieceAt(const Position& pos) {
			return m_pieceArray[(pos.getRow()-1)*8 + (pos.getColumn().number -1)];
		}
		const Piece* getPieceAt(const Position& pos) const {
			return m_pieceArray[(pos.getRow()-1)*8 + (pos.getColumn().number -1)];
		}

		~Board();
	};
}
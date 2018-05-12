#pragma once
#include "Piece.h"
#include "PiecesH.h"
#include <vector>

namespace ch {
	class Board {
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

		void placePieceAt(Color color, const Position& pos, PieceType pieceType) {
			Piece*& posPtr = m_at(pos);
			if(posPtr == nullptr) {
				try {
					switch (pieceType)
					{
					case ch::PieceType::PAWN:
						posPtr = new Pawn(color, pos);
						break;
					case ch::PieceType::BISHOP:
						posPtr = new Bishop(color, pos);
						break;
					case ch::PieceType::KNIGHT:
						posPtr = new Knight(color, pos);
						break;
					case ch::PieceType::ROOK:
						posPtr = new Rook(color, pos);
						break;
					case ch::PieceType::QUEEN:
						posPtr = new Queen(color, pos);
						break;
					case ch::PieceType::KING:
						posPtr = new King(color, pos);
						break;
					}
				}
				catch(...) {
					delete posPtr;
					posPtr = nullptr;
					throw;
				}
				if (color == Color::BLACK){
					m_blackPieces.push_back(posPtr);
				}
				else{
					m_whitePieces.push_back(posPtr);
				}
				return;
			}
			throw std::logic_error("Destination is not nullptr.");
		}

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
#pragma once
#include "Piece.h"
#include "Pawn.h"
#include "Board.h"
#include "King.h"
namespace ch {
	class ChessEngine {
		Color m_currCol;
		//True, when valid, Piece and color check
		bool m_grabCheck(const Position& src) {  
			auto piece = m_Board.getPieceAt(src);
			return ((piece != nullptr) && (piece->getColor() == m_currCol)); //If there is a piece, and it's color is ok
		}
		//Checking for castling, en Passant, or pawn swap
		bool m_specStepProcess(const Position& src, const Position& dest);
		//Checks whether the way is free to the dest. Does not include destination.
		bool m_isWayFree(const Position& src, const Position& dest);

		const King& m_whiteKing;
		const King& m_blackKing;

		void m_calcNoCheckGrid();
		bool m_isKingInCheck(const Position& targetPos, const Position& currPos);

		bool m_activeCheck;

		bool m_blackCastled;
		bool m_whiteCastled;

		Pawn* m_delEnPassCache; //EnPass able pawn stored here, deleted in the next round
		Board& m_Board;
	public:
		ChessEngine(Board& chessBoard);
		bool ProcessStep(const Position& src, const Position& dst);
		const Color& getCurrentColor();
		~ChessEngine(void);
	};

}
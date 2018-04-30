#include "ChessEngine.h"

namespace ch {

	bool ChessEngine::m_specStepProcess(const Position& src, const Position& dest) {
		auto srcPiece = m_Board.getPieceAt(src);
		//Castling check and process
		if((m_currCol == Color::WHITE && !m_whiteCastled) || (m_currCol == Color::BLACK && !m_blackCastled)) { //There is ability to castle
			if(srcPiece->pieceType == PieceType::KING) { //King is chosen
				if((m_currCol == Color::WHITE) && (dest.getRow() == 1)) { //White, in the first row
					//Short castling, E to G and the way is free between the king and the rook: 
					if(dest.getColumn().letter == Column::CL::G && m_isWayFree(src, Position(Column::CL::H, 1))) {
						//TODO
					}
				}				
			}
		}
		
		return true;
	}

	bool ChessEngine::m_isWayFree(const Position& src, const Position& dest) {
		return true;
	}

	void ChessEngine::m_calcNoCheckGrid() {
		const King& cCKing = (m_currCol == Color::WHITE)? m_whiteKing : m_blackKing;
		cCKing.noCheckGrid.clear(); //Clearing it's grid first
		if (!m_activeCheck){ //The current player is not in check
			auto mvGridKing = cCKing.getMoveGrid();
			//For the KING
			for (auto it = mvGridKing.begin(); it < mvGridKing.end(); it++) {
				if(!m_isKingInCheck(*it, cCKing.getPosition())) { //When the king is not in check, iterating through the move grid
					cCKing.noCheckGrid.push_back(*it); //Then it can move on that position
				}
			}

		}
	}

	ChessEngine::ChessEngine(Board& chessBoard) : m_Board(chessBoard)
	{
	}


	ChessEngine::~ChessEngine(void)
	{
	}
}
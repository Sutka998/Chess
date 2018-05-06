#include "ChessEngine.h"
#include <stdexcept>

namespace ch {

	ChessEngine::ChessEngine(Board& chessBoard, const King& whiteKing, const King& blackKing)
		: m_Board(chessBoard), m_whiteKing(whiteKing), m_blackKing(blackKing)
	{
		m_Board.save();
	}

	bool ChessEngine::ProcessStep(const Position& src, const Position& dst) {
		Piece* srcPiece = m_Board.getPieceAt(src);
		Piece* dstPiece= m_Board.getPieceAt(dst);
		if(srcPiece == nullptr) {
			return false; //no piece
		}
		if(srcPiece->getColor() != m_currCol) {
			return false; //wrong color
		}
		if(m_castlingCheck(src, dst) == true) {
			return true; //Spec. step processed
		}
		if(srcPiece->pieceType == PieceType::KNIGHT) {

		}
		if(m_isWayFree(src, dst)) {
			if(srcPiece->canMoveHit(dst, MovType::MOVE)) {
				if(dstPiece != nullptr) {
					return false; //Invalid step
				}
			} 
			else if(srcPiece->canMoveHit(dst, MovType::HIT))  {
				if(dstPiece == nullptr || dstPiece->pieceType ==  PieceType::KING) {
					return false; //We can not hit the nothing, or we can not hit the king
				}

			}
		}
	}

	bool ChessEngine::m_castlingCheck(const Position& src, const Position& dest) {
		Piece* srcPiece = m_Board.getPieceAt(src);
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

	bool ChessEngine::m_tryStepExecute(const Position& src, const Position& dest, MovType mvType) {
		Piece* srcPiece = m_Board.getPieceAt(src);
		if(mvType == MovType::MOVE) {
			m_Board.movePiece(src, dest);
		} else {
			m_Board.deletePieceAt(dest);
			m_Board.movePiece(src, dest);
		}
		m_checkEvaluate(); //The step was completed on the board, checking for check
		if(m_activeCheck) {
			m_Board.undo(); 
			return false; //Invalid step
		} else {
			srcPiece->Move_Hit(dest); //Completing the move/hit by moving the piece to the destination
		}
		//Move completed
		if(srcPiece->pieceType == PieceType::PAWN) {
			//We moved a W pawn from the 7th, or a B pawn from the 2nd row => pawn swap
			if((m_currCol == Color::WHITE && src.getRow() == 7) || (m_currCol == Color::BLACK && src.getRow() == 2)) {
				//Valid pawn swap
				//Event should be triggered //TODO
				PieceType p;
				m_Board.deletePieceAt(dest); //Remove our pawn
				m_Board.placePieceAt(m_currCol, dest, p); //Replacing with the asked piece		
				//Valid step happened
			}
		}
		return true;
	}

	bool ChessEngine::m_isWayFree(const Position& src, const Position& dest) {
		return true;
	}

	bool ChessEngine::m_checkStraightDir(const Position& currPos, int& alliedPieces, const King& currKing) {
		Piece* currPiece = m_Board.getPieceAt(currPos);
		if((currPiece != nullptr)) {
			if(currPiece->getColor() == m_currCol) { //Allied piece
				alliedPieces++;
				return false;
			} 
			else if(alliedPieces > 0) { //Enemy piece found, and allied pieces were found between
				return true; //No check from this direction, enemy piece was found, loop should be broken.
			}
			else { //No allied piece was between, the king can be in check
				if (m_Board.getPieceAt(currPos)->canMoveHit(currKing.getPosition(), MovType::HIT)) { //If the enemy piece can hit our king
					//It is check then
					m_activeCheck = true;		 
				}
				return true; //Check - no check evaluated. The outer loop should be broken.
			}
		}
		return false; //Nothing was found, continuing the loop
	}

	void ChessEngine::m_checkKnightDir(const King& currKing) {
		//8 different directions to check
		intPair argArray[8] = {{1,2}, {-1, 2}, {2, 1}, {2, -1}, {1,-2}, {-1, -2}, {-2, 1}, {-2, -1}};
		// North right-left ; east up-down ; south right-left ; west up-down
		const Position& kingPos = currKing.getPosition();
		for (int i = 0; i < 8; i++) {
			int cl = kingPos.getColumn().number;
			int rw = kingPos.getRow();
			if((cl+argArray[i].a >=1) && (cl+argArray[i].a <=8) //Valid column value
				&& (rw+argArray[i].b >=1) && (rw+argArray[i].b <=8)) //Valid row value
			{
				//The position is valid
				Position iterPos(kingPos.getColumn()+argArray[i].a, kingPos.getRow()+argArray[i].b);
				if(m_Board.getPieceAt(iterPos)->pieceType == PieceType::KNIGHT &&
					m_Board.getPieceAt(iterPos)->getColor() != m_currCol) //Enemy knight can hit our king
				{ 
					m_activeCheck = true; //King is in check
					return;
				}
			}

		}
	}

	void ChessEngine::m_checkEvaluate() {
		m_activeCheck = false;
		const King& currKing = (m_currCol == Color::WHITE) ? m_whiteKing : m_blackKing;
		m_checkKnightDir(currKing);
		if(m_activeCheck == true) {
			return; //There is check found, we can return
		}
		int alliedPieces = 0;

		for (int i = 0; i < 8; i++) {
			auto iter = lineITER(static_cast<lineITER::iterDIR>(i), currKing.getPosition());
			iter++;
			for(; !iter.isFinished(); iter++) {
				if(m_checkStraightDir((*iter), alliedPieces, currKing) == true) {
					break;
				}
			}
			alliedPieces = 0;
		}
	}

	ChessEngine::~ChessEngine(void)
	{
	}

	void ChessEngine::lineITER::operator++(int) {
		try {
			switch (m_iterDirection)
			{
			case iterDIR::UP:
				m_currPos = Position(m_currPos.getColumn(), m_currPos.getRow() +1);
				break;
			case iterDIR::DOWN:
				m_currPos = Position(m_currPos.getColumn(), m_currPos.getRow() -1);
				break;
			case iterDIR::RIGHT:
				m_currPos = Position(m_currPos.getColumn().number+1, m_currPos.getRow());
				break;
			case iterDIR::LEFT:
				m_currPos = Position(m_currPos.getColumn().number -1, m_currPos.getRow());
				break;
			case iterDIR::UPRIGHT:
				m_currPos = Position(m_currPos.getColumn().number +1, m_currPos.getRow() +1);
				break;
			case iterDIR::UPLEFT:
				m_currPos = Position(m_currPos.getColumn().number +1, m_currPos.getRow() -1);
				break;
			case iterDIR::DOWNRIGHT:
				m_currPos = Position(m_currPos.getColumn().number -1, m_currPos.getRow() +1);
				break;
			case iterDIR::DOWNLEFT:
				m_currPos = Position(m_currPos.getColumn().number -1, m_currPos.getRow() -1);
				break;
			default:
				break;
			}
		}
		catch(std::invalid_argument) {
			m_finished = true;
		}
	}
}
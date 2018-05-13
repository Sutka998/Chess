#include "ChessEngine.h"
#include <stdexcept>

namespace ch {

	ChessEngine::ChessEngine(Board& chessBoard, const King& whiteKing, const King& blackKing)
		: m_Board(chessBoard), m_whiteKing(whiteKing), m_blackKing(blackKing), gameFlags(m_flags)
	{
		m_currCol = Color::WHITE;
		m_Board.save();
	}

	bool ChessEngine::ProcessStep(const Position& src, const Position& dst) {
		Piece* srcPiece = m_Board.getPieceAt(src);
		Piece* dstPiece= m_Board.getPieceAt(dst);
		bool validHappened = false;
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
			if(m_tryKnightStep(src, dst) == false) {
				return false; //The knight step didn't complete
			}
			validHappened = true; //Knight step executed
		}
		else if(m_isWayFree(src, dst)) {
			if(dstPiece == nullptr) { //Moving, dest is empty
				if(srcPiece->canMoveHit(dst, MovType::MOVE)) {
					if (m_tryStepExecute(src, dst, MovType::MOVE) == false) { //Destination is nullptr
						return false; //Invalid step
					}
					validHappened = true; //We have executed the move step
				}
			}
			else if((dstPiece->getColor() != m_currCol) && (dstPiece->pieceType !=  PieceType::KING))  {
				//Destination is enemy, and not the king
				if(srcPiece->canMoveHit(dst, MovType::HIT)) { //Our piece is able to hit the enemy 
					if(m_tryStepExecute(src, dst, MovType::HIT) == false) {
						return false; 
					}
					validHappened = true; //Hit step executed
				}
			}
		}
		if (validHappened) { //When valid step happened, we have to save,
			m_roundEnd();
			return true;
		}
		return false;
	}

	bool ChessEngine::m_castlingCheck(const Position& src, const Position& dest) {
		Piece* srcPiece = m_Board.getPieceAt(src);
		//Castling check and process
		if(srcPiece->isMoved() || m_flags.isCheck) {
			return false;
		}
		unsigned row = (m_currCol == Color::WHITE) ? 1 : 8;
		if((srcPiece->pieceType == PieceType::KING) ) { //King is chosen
			if(dest.getRow() == row) { //We are moving on that row, what is corresponds to the player's color.
				//King side castle check, are we moving on the G, and the way is free?
				if(dest.getColumn().letter == Column::CL::G && m_isWayFree(src, Position(Column::CL::H, row))){ 
					if(m_tryCastle(KINGSIDE, srcPiece)) { return true;} //If the castling was successful, we return
				}
				//Queen side castle check
				else if(dest.getColumn().letter == Column::CL::C && m_isWayFree(src, Position(Column::CL::A, row))) {
					if(m_tryCastle(QUEENSIDE, srcPiece)) { return true;}
				}
				else {
					return false; //No, we aren't castling
				}
			}				
		}
		return false;
	}

	bool ChessEngine::m_tryCastle(side_t side, Piece* kingCstl) {
		unsigned row = (m_currCol == Color::WHITE)? 1 : 8;
		Position kingFrom(Column::CL::E, row);
		Position kingTo, rookFrom, rookTo;
		if(side == KINGSIDE) { //Setting up the positions
			kingTo = Position(Column::CL::G, row);
			rookFrom = Position(Column::CL::H, row);
			rookTo = Position(Column::CL::F, row);
		} else {
			kingTo = Position(Column::CL::C, row);
			rookFrom = Position(Column::CL::A, row);
			rookTo = Position(Column::CL::D, row);
		}
		Piece* rookCstl = m_Board.getPieceAt(rookFrom);
		if(rookCstl == nullptr || rookCstl->isMoved() || rookCstl->pieceType != PieceType::ROOK) {
			return false; //No piece, or rook is moved, or the rookCstl is not rook						
		}
		else {
			m_Board.movePiece(kingFrom, rookTo); //Step one with the king, rookTo field is next to the king
			m_checkEvaluate(); //The king shouldn't cross "check"
			if(m_activeCheck) {
				m_Board.undo();
				return false;
			}
			m_Board.movePiece(rookTo, kingTo); //Moving the king to the final position (it was on the rookTo field)
			m_Board.movePiece(rookFrom, rookTo); //Moving the rook to its position
			m_checkEvaluate();
			if(m_activeCheck) { //Check occurred
				m_Board.undo();
				return false;
			}
			//Movement was ok, step completed
			kingCstl->Move_Hit(kingTo);
			rookCstl->Move_Hit(rookTo);
			return true;
		}
	}

	bool ChessEngine::m_tryStepExecute(const Position& src, const Position& dest, MovType mvType) {
		Piece* srcPiece = m_Board.getPieceAt(src);
		if(mvType == MovType::MOVE) {
			m_Board.movePiece(src, dest);
		} else {
			m_Board.deletePieceAt(dest);
			m_Board.movePiece(src, dest);
		}
		if(srcPiece->pieceType == PieceType::KING) {
			srcPiece->Move_Hit(dest); //We have to pre-remove the king, for correct check evaluation, if we are moving king.
		}
		m_checkEvaluate(); //The step was completed on the board, checking for check
		if(m_activeCheck) {
			if (srcPiece->pieceType == PieceType::KING) {
				srcPiece->Move_Hit(src); //Moving back our piece
			}
			m_Board.undo(); 
			return false; //Invalid step
		} else if(srcPiece->pieceType != PieceType::KING){ //If we have king, the move is already completed.
			srcPiece->Move_Hit(dest); //Completing the move/hit by moving the piece to the destination
		}
		//Move completed, pawn swap check, or EnPassant ability check
		if(srcPiece->pieceType == PieceType::PAWN) {
			if ((static_cast<Pawn*> (srcPiece)->m_canBeEnPass)) { //The EnPassant flag has been set
				//TODO
			}
			//We moved a W pawn from the 7th, or a B pawn from the 2nd row => pawn swap
			if((m_currCol == Color::WHITE && src.getRow() == 7) || (m_currCol == Color::BLACK && src.getRow() == 2)) {
				//Valid pawn swap
				//Event should be triggered //TODO
				PieceType p = PieceType::QUEEN;
				m_Board.deletePieceAt(dest); //Remove our pawn
				m_Board.placePieceAt(m_currCol, dest, p); //Replacing with the asked piece		
				//Valid step happened
			}
		}
		return true;
	}

	bool ChessEngine::m_tryKnightStep(const Position& src, const Position& dest) {
		Piece* currKnight = m_Board.getPieceAt(src);

		if(currKnight->canMoveHit(dest, MovType::MOVE)) { //Move and hit grid is the same for the knight
			if(m_Board.getPieceAt(dest) == nullptr) { //Destination is empty => moving the piece
				m_Board.movePiece(src, dest);
			}
			else if(m_Board.getPieceAt(dest)->pieceType == PieceType::KING) { //We can't hit 
				return false;//Invalid, we can't hit a king
			}
			else if(m_Board.getPieceAt(dest)->getColor() != m_currCol) { //Hitting enemy piece, if he is enemy colored.
				m_Board.deletePieceAt(dest);
				m_Board.movePiece(src, dest);
			}
			else {
				return false; //Invalid step, trying to hit allied piece, or a king
			}
			m_checkEvaluate();
			if(m_activeCheck) {//Gets check after move, invalid step
				m_Board.undo();
				return false;
			}
			currKnight->Move_Hit(dest);
			return true;
		}
		else { return false;} //Not in the hit/move grid, invalid step.
		//Check check
		m_checkEvaluate();
		if(m_activeCheck) {
			m_Board.undo();
			return false; //We got check, we can not step this way, and we have to undo.
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
			int cl = kingPos.getColumn().number; //Kings column
			int rw = kingPos.getRow(); //Kings row
			if((cl+argArray[i].a >=1) && (cl+argArray[i].a <=8) //Valid column value, column shifting
				&& (rw+argArray[i].b >=1) && (rw+argArray[i].b <=8)) //Valid row value, row shifting
			{
				//The position is valid
				Position iterPos(kingPos.getColumn()+argArray[i].a, kingPos.getRow()+argArray[i].b);
				if(m_Board.getPieceAt(iterPos) != nullptr) { //Valid enemy piece found
					if(m_Board.getPieceAt(iterPos)->pieceType == PieceType::KNIGHT &&
						m_Board.getPieceAt(iterPos)->getColor() != m_currCol) //Enemy knight can hit our king
					{ 
						m_activeCheck = true; //King is in check
						return;
					}
				}
			}

		}
	}

	void ChessEngine::m_checkEvaluate() {
		m_activeCheck = false;
		const King& currKing = (m_currCol == Color::WHITE)? m_whiteKing : m_blackKing;

		m_checkKnightDir(currKing);
		if(m_activeCheck == true) {
			return; //There is check found, we can return
		}
		int alliedPieces = 0;

		for (int i = 0; i < 8; i++) {
			auto iter = lineITER(static_cast<lineITER::iterDIR>(i), currKing.getPosition());//We need the position dependent on the board
			iter++;
			for(; !iter.isFinished(); iter++) {
				if(m_checkStraightDir((*iter), alliedPieces, currKing) == true) {
					break;
				}
			}
			if(m_activeCheck) { //If check found, we can return
				return;
			}
			alliedPieces = 0;
		}
	}

	void ChessEngine::m_roundEnd() {
		m_currCol = (m_currCol == Color::WHITE) ? Color::BLACK : Color::WHITE; //Enemy's next turn, swapping player
		m_checkEvaluate(); //Evaluate check and mate situations for the enemy
		//Setting up the flags
		m_flags.isMate = m_checkForMate();
		m_flags.isCheck = m_activeCheck;
		m_Board.save(); //Saving the board for the next turn
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
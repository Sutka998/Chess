#include "ChessEngine.h"
#include <stdexcept>

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

	void ChessEngine::m_calcWhereCanStep() {
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
			//Direction checking

			//For the others
			auto currIter = (m_currCol == Color::WHITE)? m_Board.it_whitesBegin() : m_Board.it_blacksBegin();
			auto endIter = (m_currCol == Color::WHITE)? m_Board.it_whitesEnd() : m_Board.it_blacksEnd();
			for (; currIter != endIter; currIter++) { //Going through the current player's pieces
				if((*currIter)->pieceType != PieceType::KING) { //Not king

				}
			}
		}
	}

	void ChessEngine::m_calcNoCheckGrid(King* currKing) { //Lines, where one allied piece blocks from check
		//Straight directions
		std::vector<Position> checkLine; //The line, or the field from where the check comes
		Position kingPos = currKing->getPosition();

		for (int Index = 0; Index < 8 ; Index++) {
			lineITER::iterDIR iterDirection = static_cast<lineITER::iterDIR>(Index);
			lineITER it(iterDirection, kingPos);
			it++; //We are starting the process from the next field, so we increase it once
			while(!it.isFinished()){
				const Position& currPos = *it;
				if(m_dirCalculation(checkLine, kingPos, currPos)){
					break;
				}
				it++;
			}
		}
	}

	bool ChessEngine::m_dirCalculation(std::vector<Position>& checkLine, const Position& kingPos, const Position& currPos) {
		std::vector<Piece*> alliedPieces;
		std::vector<Position> linePositions;
		Piece* currPiece = m_Board.getPieceAt(currPos);
		linePositions.push_back(currPos); //Saving positions till the first black(included) (or end of the board)
		if((currPiece != nullptr)) {
			if(currPiece->getColor() == m_currCol) { //Allied piece
				alliedPieces.push_back(currPiece);
				return false;
			} 
			else if(alliedPieces.size()>1) { //Enemy piece found, and more than one allied pieces were found between
				for (int i = 0; i < alliedPieces.size() ; i++) {
					alliedPieces[i]->noCheckG_isnotapplied = true; //They can move, where they want, there is no check-grid restriction applied
				}
			}
			else if(alliedPieces.size() == 1) { //One allied piece found: It should move on line. Applying noCheckGrid 
				if(currPiece->canMoveHit(kingPos, MovType::HIT)) { //When that one piece blocks from check (enemy piece can hit the king, without allied)
					alliedPieces[0]->noCheckG_isnotapplied = false; //We evaluate the check grid
					alliedPieces[0]->noCheckGrid.clear();
					vectorIntSect(alliedPieces[0]->getMoveGrid(), linePositions, alliedPieces[0]->noCheckGrid); //Intersecting movgrid with the line
					vectorIntSect(alliedPieces[0]->getHitGrid(), linePositions, alliedPieces[0]->noCheckGrid); //Intersecting hitgrid with the line
				}
				else { //If can't hurt our king, there is no movement restriction
					alliedPieces[0]->noCheckG_isnotapplied = true;
				}
			}
			else { //No allied piece was between, the king can be in check
				if(currPiece->canMoveHit(kingPos, MovType::HIT)) { //Check from line direction
					m_activeCheck = true; //The player is in check
					checkLine = linePositions; //Saving the check line
				}
				m_activeCheck = false;
			}
			return true; //Enemy piece was found, for loop should be broken;
		}
		return false; //Nothing was found, continuing the loop

	}

	ChessEngine::ChessEngine(Board& chessBoard, const King& whiteKing, const King& blackKing)
		: m_Board(chessBoard), m_whiteKing(whiteKing), m_blackKing(blackKing)
	{
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
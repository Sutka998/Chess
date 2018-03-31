#include "Pawn.h"
namespace ch {

	Pawn::Pawn(Color color, const Position& pos)
		: Piece(color, pos, PieceName::PAWN), m_canBeEnPass(false)
	{
		m_evalMoveGrid(); //It should be done here, for the first time.
		m_evalHitGrid();
	}

	void Pawn::m_evalMoveGrid() {
		//We evaluated it since the last step
		if(m_mGridEvaled) { //We have nothing to do here.
			return;
		}
		//Clearing the grid, because probably a new step happened, and have to give new details
		m_moveGrid.clear();

		if(m_color == Color::WHITE) { //WHITE PAWN****
			m_mGridAdd_white();
		}
		else {//BLACK PAWN**** same things, counted from the 8th row, backwards
			m_mGridAdd_black();
		}
		m_mGridEvaled = true; //It has just been evaluated.
		return;
	}
	void Pawn::m_evalHitGrid() {
		if(m_hGridEvaled) { return;} //Its evaluated since the last step
		if(m_color == Color::WHITE) { //***WHITE, incrementing coordinates
			if(m_position.getColumn() == Column::CL::A) {
				m_hitGrid.push_back(Position(Column::CL::B, m_position.getRow()+1));
			}
			else if(m_position.getColumn() == Column::CL::H) {
				m_hitGrid.push_back(Position(Column::CL::G, m_position.getRow()+1));
			}
			else {
				Column c = m_position.getColumn();
				m_hitGrid.push_back(Position(Column::CL::G, m_position.getRow()+1));
			}
		} 
		else {//***BLACK

		}
	}

	void Pawn::m_mGridAdd_white() {
		//For white pawns, adding positions to the move grid.
		//Isn't moved: pawn stays in the second row, can move on the 3rd, or on the 4th row
		if(!m_isMoved) {
			m_moveGrid.push_back(Position(m_position.getColumn(), 3)); //3rd
			m_moveGrid.push_back(Position(m_position.getColumn(), 4)); //4th
		} //If it is moved, and not staying on the 8th row:
		else if (m_position.getRow() != 8){
			//It can step on the next row only.
			m_moveGrid.push_back(Position(m_position.getColumn(), m_position.getRow()+1));
		}
	}
	void Pawn::m_mGridAdd_black(){
		//For the black pawn, adding the positions to the move grid.
		if(!m_isMoved) {
			m_moveGrid.push_back(Position(m_position.getColumn(), 6)); //3rd from the 8th
			m_moveGrid.push_back(Position(m_position.getColumn(), 5)); //4th from the 8th
		}
		else if (m_position.getRow() != 1){
			//It can step on the next row only. Decrementing step, from the start row.
			m_moveGrid.push_back(Position(m_position.getColumn(), m_position.getRow()-1));
		}
	}

	bool Pawn::Move_Hit(const Position& pos) {
		//Not moved, and steps 2 ahead, able to hit by En Passant
		if(!m_isMoved) { 
			//****White, and stepping on the 4th row.
			if ((m_color == Color::WHITE) && (pos.getRow() == 4)) {
				if(Piece::Move_Hit(pos)) { //It means, it could move, and it is moved.
					m_canBeEnPass = true;
					return true; //Move complete
				}
				return false; //Got some nasty position, like B2 to C4 (When column mismatches)
			}
			//****Black, and stepping on the 5th row.
			else if(pos.getRow() == 5) {
				if(Piece::Move_Hit(pos)) { //It means, it could move, and it is moved.
					m_canBeEnPass = true;
					return true; //Move complete
				}
				return false;
			}
		}
		//Other cases: Just move, and look
		return Piece::Move_Hit(pos);
	}
}
#include "Pawn.h"
namespace ch {

	Pawn::Pawn(Color color, const Position& pos)
		: Piece(color, pos, PieceType::PAWN), m_canBeEnPass(false)
	{
		m_evalMoveGrid(); //It should be done here, for the first time.
		m_evalHitGrid();
	}

	const PieceType Pawn::pieceType = PieceType::PAWN;

	void Pawn::m_evalMoveGrid() {
		//We evaluated it since the last step
		if(m_mGridEvaled) { //We have nothing to do here.
			return;
		}
		//Clearing the grid, because probably a new step happened, and have to give new details
		m_moveGrid.clear();

		if(m_color == Color::WHITE) { //WHITE PAWN****
			m_moveGridAdd_white();
		}
		else {//BLACK PAWN**** same things, counted from the 8th row, backwards
			m_moveGridAdd_black();
		}
		m_mGridEvaled = true; //It has just been evaluated.
		return;
	}
	void Pawn::m_evalHitGrid() {
		/*First and last row shouldn't be checked, due to promotion. */
		if(m_hGridEvaled) { return;} //Its evaluated since the last step
		m_hitGrid.clear();
		if(m_color == Color::WHITE) { //***WHITE, incrementing coordinates
			m_hitGridAdd_white();			
		} 
		else {//***BLACK
			m_hitGridAdd_black();
		}
		m_hGridEvaled = true;
		return;
	}

	void Pawn::m_moveGridAdd_white() {
		//For white pawns, adding positions to the move grid.
		//Isn't moved: pawn stays in the second row, can move on the 3rd, or on the 4th row
		if(!m_isMoved) {
			m_moveGrid.push_back(Position(m_position.getColumn(), 3)); //3rd
			m_moveGrid.push_back(Position(m_position.getColumn(), 4)); //4th
		} //If it is moved. Can't stay in the 8th row, because it would be swapped.
		else {
			//It can step on the next row only.
			m_moveGrid.push_back(Position(m_position.getColumn(), m_position.getRow()+1));
		}
	}
	void Pawn::m_moveGridAdd_black(){
		//For the black pawn, adding the positions to the move grid.
		if(!m_isMoved) {
			m_moveGrid.push_back(Position(m_position.getColumn(), 6)); //3rd from the 8th
			m_moveGrid.push_back(Position(m_position.getColumn(), 5)); //4th from the 8th
		} //If it is moved. Can't stay in the 1st row, because it would be swapped.
		else{
			//It can step on the next row only. Decrementing step, from the start row.
			m_moveGrid.push_back(Position(m_position.getColumn(), m_position.getRow()-1));
		}
	}

	void Pawn::m_hitGridAdd_white() {
		const Column& c = m_position.getColumn();
		if(c == Column::CL::A) {//Left column
			m_hitGrid.push_back(Position(Column::CL::B, m_position.getRow()+1));
		}
		else if(c == Column::CL::H) {//Right column
			m_hitGrid.push_back(Position(Column::CL::G, m_position.getRow()+1));
		}
		else {//Not side
			m_hitGrid.push_back(Position(c - 1, m_position.getRow()+1));
			m_hitGrid.push_back(Position(c + 1, m_position.getRow()+1));
		}
	}
	void Pawn::m_hitGridAdd_black() {
		const Column& c = m_position.getColumn();
		if(c == Column::CL::A) {//Left column
			m_hitGrid.push_back(Position(Column::CL::B, m_position.getRow()-1));
		}
		else if(c == Column::CL::H) {//Right column
			m_hitGrid.push_back(Position(Column::CL::G, m_position.getRow()-1));
		}
		else {//Not side
			m_hitGrid.push_back(Position(c - 1, m_position.getRow()-1));
			m_hitGrid.push_back(Position(c + 1, m_position.getRow()-1));
		}
	}

	void Pawn::Move_Hit(const Position& pos) {
		
		if(canMoveHit(pos, MovType::MOVE)){
			if(!m_isMoved) { //If not moved, and steps 2 ahead, it is able to hit by En Passant
				//****White, and stepping on the 4th row.
				if ((m_color == Color::WHITE) && (pos.getRow() == 4)) {
					m_canBeEnPass = true; //We are moving in the 4th row
					m_isMoved = true;
					return; //Move complete
				}
				//****Black, and stepping on the 5th row.
				else if(pos.getRow() == 5) {
					m_canBeEnPass = true;
					m_isMoved = true;
					return; //Move complete
				}
			}
		}
		//Other cases: When the piece is already moved, or moving/hitting differently:
		m_canBeEnPass = false; //
		Piece::Move_Hit(pos);
	}
}
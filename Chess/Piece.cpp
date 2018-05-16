#include "Piece.h"

namespace ch {

	std::ofstream& operator<<(std::ofstream& os, PieceType pc) {
		switch (pc)
		{
		case ch::PieceType::PAWN:
			os<<"PAWN";
			break;
		case ch::PieceType::BISHOP:
			os<<"BISHOP";
			break;
		case ch::PieceType::KNIGHT:
			os<<"KNIGHT";
			break;
		case ch::PieceType::ROOK:
			os<<"ROOK";
			break;
		case ch::PieceType::QUEEN:
			os<<"QUEEN";
			break;
		case ch::PieceType::KING:
			os<<"KING";
			break;
		}
		return os;
	}
	std::ifstream& operator>>(std::ifstream& is, PieceType& pc) {
		std::string a;
		is>>a;
		if(a == "BISHOP") {
			pc = PieceType::BISHOP;
		}
		else if(a == "KING") {
			pc = PieceType::KING;
		}
		else if(a == "KNIGHT") {
			pc = PieceType::KNIGHT;
		}
		else if(a == "ROOK") {
			pc = PieceType::ROOK;
		}
		else if(a == "PAWN") {
			pc = PieceType::PAWN;
		}
		else if(a == "QUEEN") {
			pc = PieceType::QUEEN;
		}
		else {
			is.setstate(std::ios::failbit);
		}
		return is;
	}

	Piece::Piece(Color color, const Position& position, PieceType pcType) 
		: m_color(color), m_position(position),	m_isMoved(false), pieceType(pcType),
		m_mGridEvaled(false), m_hGridEvaled(false), m_isCacheValid(false)
	{	}

	bool Piece::canMoveHit(const Position& pos, MovType mvtyp) const {
		//Is there any cached value
		if(m_isCacheValid && (pos == m_canMoveHitCache)){
			return true;
		}
		if(m_isInGrid(mvtyp, pos)) { //Then check for the grid elements
			m_canMoveHitCache = pos;
			m_isCacheValid = true;
			return true;
		}
		return false;
	} 

	bool Piece::m_isInGrid(MovType mvtype, const Position& pos) const {
		auto m_grid = &m_hitGrid;
		if(mvtype == MovType::MOVE)	{m_grid = &m_moveGrid;}

		for (auto iter = m_grid->begin(); iter != m_grid->end(); iter++)	{ 
			if(*iter == pos) return true;
		}
		return false;
	}
	void Piece::m_move(const Position& pos) {
		m_position = pos;
		m_mGridEvaled = m_hGridEvaled = false;//The grids no longer evaluated, it needs to be false, for refreshing the grids.
		m_evalHitGrid(); //Moved, refreshing the grids.
		m_evalMoveGrid();
		m_isCacheValid = false; //the cached moving position is no longer valid.
	}

	void Piece::m_hGridEqMvGrid() {
		if(m_hGridEvaled) {
			return;
		}
		m_hitGrid.clear();
		m_evalMoveGrid();
		m_hitGrid = m_moveGrid;
		m_hGridEvaled = true;
		return;
	}

	void Piece::Move_Hit(const Position& pos) {
		if( canMoveHit(pos, MovType::MOVE) || canMoveHit(pos, MovType::HIT) ) {
			m_move(pos);
			m_isMoved = true; //We moved the piece.
			return;
		}
		throw std::invalid_argument("Piece::Move_Hit Position is not in the grid. Unable to move the piece.");
	}

	void Piece::Serialize(std::ofstream& os) const {
		os<<m_position.getColumn().number<<", "<<m_position.getRow()<<", "<<m_isMoved<<"\n";
	}

	void Piece::Deserialize(std::ifstream& is) {
		unsigned column, row;
		char c;
		is>>column;
		is>>c;
		if(is.good() && c == ','){
			is>>row;
			if(is.good()){
				m_position = Position(column, row);
				is>>c;
				is>>m_isMoved;
				if(is.good() && c == ',') {
					is>>c;
					if(is.good() && c == '\n') {
						m_evalHitGrid();
						m_evalMoveGrid();
						m_isCacheValid = false;
						return ;
					}
				}
			}
		}
		throw std::exception("File format error");
	}

}
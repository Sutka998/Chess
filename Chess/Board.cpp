#include "Board.h"

namespace ch {

	Board::Board()
	{
		for (int i = 0; i < 64; i++) {
			m_pieceArray[i] = nullptr;
		}
	}

	void Board::save() {
		for (int i = 0; i < m_deleteList.size(); i++) {
			delete m_deleteList[i]; //deleting delete list pointers
			m_deleteList.clear();
		}
		for (int i = 0; i < 64; i++) {
			m_savedArray[i] = m_pieceArray[i]; //Saving the data
		}
	}

	void Board::undo() {
		for (int i = 0; i < m_deleteList.size(); i++) {
			//Putting back the piece to the Piece list
			((m_deleteList[i]->getColor() == Color::WHITE) ? m_whitePieces : m_blackPieces).push_back(m_deleteList[i]);
		}
		m_deleteList.clear();
		for (int i = 0; i < 64; i++) {
			m_pieceArray[i] = m_savedArray[i]; //Loading back the data
		}
	}

	void Board::Serialize(std::ofstream& os) {
		os<<"BOARD: ";
		save();
		for (int i = 0; i<64; i++) {
			if(m_pieceArray[i] == nullptr) {
				os<<"NOPE, ";
			} else {
				os<<"PIECE ";
				os<<m_pieceArray[i]->getColor() <<" ";
				os<<m_pieceArray[i]->pieceType<<" , ";
				m_pieceArray[i]->Serialize(os);
			}
		}
	}

	void Board::Deserialize(std::ifstream& is) {
		std::string buff; Color col; PieceType pcTyp;
		save();
		is>>buff;
		if(buff == "BOARD:") {
			m_whitePieces.clear();
			m_blackPieces.clear();
			for (int i = 0; i < 64; i++) {
				is>>buff;
				if(buff == "NOPE,") {
					delete m_pieceArray[i];
					m_pieceArray[i] = nullptr;
				}
				else if(buff == "PIECE") {
					is>>col>>pcTyp;
					if(is.good()){
						int x = i/8;
						int y = i%8;
						delete m_pieceArray[i];
						m_pieceArray[i] = nullptr;
						placePieceAt(col, Position(y+1,x+1), pcTyp);
						char c = 0;
						is>>c;
						if(is.good() && c == ',') {
							m_pieceArray[i]->Deserialize(is);
						}
						if(!is.good()) {
							throw std::exception("File format error\n");
						}
					}
				}
			}
			save();
			return;
		}
		throw std::exception("File format error\n");
	}

	void Board::placePieceAt(Color color, const Position& pos, PieceType pieceType) {
		Piece*& piecePtr = m_at(pos);
		if(piecePtr == nullptr) {
			try {
				switch (pieceType)
				{
				case ch::PieceType::PAWN:
					piecePtr = new Pawn(color, pos);
					break;
				case ch::PieceType::BISHOP:
					piecePtr = new Bishop(color, pos);
					break;
				case ch::PieceType::KNIGHT:
					piecePtr = new Knight(color, pos);
					break;
				case ch::PieceType::ROOK:
					piecePtr = new Rook(color, pos);
					break;
				case ch::PieceType::QUEEN:
					piecePtr = new Queen(color, pos);
					break;
				case ch::PieceType::KING:
					piecePtr = new King(color, pos);
					break;
				}
			}
			catch(...) {
				delete piecePtr;
				piecePtr = nullptr;
				throw;
			}
			if (color == Color::BLACK){
				m_blackPieces.push_back(piecePtr);
			}
			else{
				m_whitePieces.push_back(piecePtr);
			}
			return;
		}
		throw std::logic_error("Destination is not nullptr.");
	}

	void Board::deletePieceAt(const Position& pos) {
		Piece*& piecPtr = m_at(pos);
		if(piecPtr != nullptr) {
			std::vector<Piece*>& piecesV = (piecPtr->getColor() == Color::WHITE)? m_whitePieces : m_blackPieces;
			for(auto it = piecesV.begin(); it != piecesV.end(); it++) {
				if((*it) == piecPtr) {//The piece's pointer found in the piece list
					piecesV.erase(it);
					m_deleteList.push_back(piecPtr);
					piecPtr = nullptr;
					return;
				}
			}
			throw std::logic_error("The piece that should be deleted is not in the vector");
		}

	}

	void Board::movePiece(const Position& from, const Position& dest) {
		Piece*& destPtr = m_at(dest);
		if(destPtr == nullptr) {
			Piece*& fromPtr = m_at(from);
			destPtr = fromPtr;
			fromPtr = nullptr;
			return;
		}
		throw std::logic_error("Destination is not nullptr.");
	}

	void Board::operator=(const Board& that) {
		save();
		m_whitePieces.clear();
		m_blackPieces.clear();
		for(int i = 0; i<64; i++) {
			delete m_pieceArray[i];
			m_pieceArray[i] = nullptr;
			if(that.m_pieceArray[i] != nullptr) {
				placePieceAt(that.m_pieceArray[i]->getColor(), that.m_pieceArray[i]->getPosition(), that.m_pieceArray[i]->pieceType);
			}
		}
		save();
	}

	Board::~Board() {
		save();
		for (int i = 0; i < 64 ; i++) {
			delete m_pieceArray[i];
		}

	}

}
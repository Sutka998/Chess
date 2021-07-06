#include "Board.h"

namespace ch {
	/** \brief Construct a board, with zero pieces.
	*/
	Board::Board()
	{
		for (int i = 0; i < 64; i++) {
			m_pieceArray[i] = nullptr;
		}
	}
	/** \brief Saves the board's current state.
	* \details The pieces are saved in a ghost board, and this state can be reseted until you save the board again.
	*/
	void Board::save() {
		for (int i = 0; i < m_deleteList.size(); i++) {
			delete m_deleteList[i]; //deleting delete list pointers
			m_deleteList.clear();
		}
		for (int i = 0; i < 64; i++) {
			m_savedArray[i] = m_pieceArray[i]; //Saving the data
		}
	}
	/** \brief Undoes all changes till the last save.
	* \details
	*/
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
	/** \brief Serialize the board into a given output file stream.
		*\param [in] The output file stream.
		*/
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
	/** \brief Deserialize the board from a given input file stream.
		*\details Throws exception, if the file format is not OK.
		*\param [in] The input file stream.
		*/
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
	/** \brief Places a new piece on the given position. Also adds it to the corresponding piece list.
	* \details The board should have contain nullptr on the position, where the piece is placed on. Throws exception, if you are trying to place on a not nullptr.
	*\param [in] The piece's color, what should be placed.
	*\param [in] Position of where the piece should go.
	*\param [in] The type of the piece.
	*/
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
	/** \brief Removes a piece from the corresponding position. If you try to delete a nullptr, nothing will happen.
	* \details The removed piece is stored in a temporaly storage, and upon and undo() it could be reseted, but after a save(), the temporary storage is deleted. Also removes the piece from the pieceList
	*\param [in] position to delete.
	*/
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
	/** \brief Moves a piece on the board (does not call the piece's moveHit function), the destination should be a nullptr.
	* \details If the destination is not nullptr, exception thrown. The undo() resets theese movement processes to the last save()'s status.
	*\param [in] Source position
	*\param [in] Destination position.
	*/
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
	/** \brief An operator=, to make this board equal to an another.
	* \details
	*\param [in] The another board.
	*/
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
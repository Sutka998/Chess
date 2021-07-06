#include "Graphics.h"

namespace ch{
	/**
	* \file Graphics.cpp
	* \brief Renders the game table for the user.
	*/

	/** \brief Renders the image, depending on the viewer mode.
		*/
	void Graphics::renderImg() {
		if(m_mode == graphicsMode::CONSOLE) {
			system("cls");
			m_cout<< "\t_________________________________\n";
			for (int i = 1; i <= 8 ; i++) {
				m_cout <<"\t";
				for (int j = 1; j <= 8 ; j++) {//One row, like this: | P | R |   | H |   |
					m_cout<<"| " << m_pieceDecoder(m_board->getPieceAt(Position(j, 9-i))) <<" ";
				}
				//After the line end:
				m_cout<<"|\n";
				//The closing line:
				m_cout<<"   "<<9-i<<'.'<<"\t|___|___|___|___|___|___|___|___|\n";
			}
			m_cout<<"\t  A   B   C   D   E   F   G   H  \n"; //last line
		}
		else {

		}
	}

	Graphics::~Graphics(void)
	{
	}

	char Graphics::m_pieceDecoder(const ch::Piece* piece) {
		if(piece!= nullptr) {
			bool whitePiece = (piece->getColor() == Color::WHITE);
			switch (piece->pieceType)
			{
			case PieceType::PAWN:
				if(whitePiece) return 'P'; else return 'p';
				break;
			case PieceType::ROOK:
				if(whitePiece) return 'R'; else return 'r';
				break;
			case PieceType::BISHOP:
				if(whitePiece) return 'B'; else return 'b';
				break;
			case PieceType::KNIGHT:
				if(whitePiece) return 'N'; else return 'n';
				break;
			case PieceType::KING:
				if(whitePiece) return 'K'; else return 'k';
				break;
			case PieceType::QUEEN:
				if(whitePiece) return 'Q'; else return 'q';
				break;
			}
		}
		return ' ';
	}
}
#pragma once
#include <ostream>
#include "Board.h"

namespace ch {
	class Graphics
	{
	public:
		enum class graphicsMode { CONSOLE, GRAPHICAL};
		Graphics(std::ostream& consoleOutput)
			: m_mode(graphicsMode::CONSOLE), m_cout(consoleOutput)
		{	};

		void renderImg();

		//setters, getters
		void setBoard(Board* brd) {
			m_board= brd;
		}
		const ch::Board* getBoard() {return m_board;}
		~Graphics(void);
	private:
		ch::Board* m_board;
		graphicsMode m_mode;
		std::ostream& m_cout;

		char m_pieceDecoder(const Piece* piece);
	};
}
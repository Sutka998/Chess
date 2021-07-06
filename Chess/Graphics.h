#pragma once
#include <ostream>
#include "Board.h"

namespace ch {

	/**
	* \file Graphics.h
	*/
	class Graphics
	{
	public:
		enum class graphicsMode { CONSOLE, GRAPHICAL};
		Graphics(std::ostream& consoleOutput)
			: m_mode(graphicsMode::CONSOLE), m_cout(consoleOutput)
		{	};

		void renderImg();

		/** \brief Sets the board for the console render mode.
		*\param [in] a const Board*
		*/
		void setBoard(const Board* brd) {
			m_board= brd;
		}
		/** \brief Getter for the current board.
		*/
		const ch::Board* getBoard() {return m_board;}
		~Graphics(void);
	private:
		const ch::Board* m_board;
		graphicsMode m_mode;
		std::ostream& m_cout;

		char m_pieceDecoder(const Piece* piece);
	};
}
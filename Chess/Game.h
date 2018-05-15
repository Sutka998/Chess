#pragma once
#include "PiecesH.h"
#include "Piece.h"
#include "position.h"
#include <iostream>
#include <list>
#include <string>
#include <fstream>
#include "Board.h"
#include "ChessEngine.h"
#include "Graphics.h"
#include <sstream>

namespace ch {
	inline void strToLower(std::string& str) {
		for (int i = 0; i <str.length(); i++) {
			str[i] = tolower(str[i]);
		}
	}
	//Zero, when invalid char comes
	inline unsigned toNum1_8(char c) {
		if(c >= 'a' && c <= 'h') {
			return c-'a'+1;
		}
		if(c >= 'A' && c <= 'H'){
			return c-'A'+1;
		}
		return 0;
	}

	class Game	{
	private:
		enum class startPos{CUSTOM, SAVED, BASIC};
		enum class commands{NOPE, EXIT, RESTART, SAVE, LOAD};

		Board m_board;
		ChessEngine m_engine;
		Graphics m_graphics;
		commands m_commandCache;

		void m_createBoard(startPos);
		void m_genBasicBoard();
		void m_loop();
		void m_processUserIn();
		void m_makeStep(const Position& src, const Position& dest);
		void m_processCommand();

		static PieceType m_askSwap();
		void m_save(std::string filename);
		void m_load(std::string filename);
		
		static std::ostream& m_out;

	public:
		Game();
		void NewGame();
		~Game();
	};

}
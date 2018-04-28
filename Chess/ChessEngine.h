#pragma once
#include "Piece.h"
namespace ch {
	class ChessEngine {
		Color m_currCol;
		bool m_isIncheck();
		bool m_isValidMove();
	public:
		ChessEngine(void);
		bool Command(Position src, Position dst);
		const Color& getCurrentColor();
		~ChessEngine(void);
	};

}
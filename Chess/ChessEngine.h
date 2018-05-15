#pragma once
#include "Piece.h"
#include "Pawn.h"
#include "Board.h"
#include "King.h"

namespace ch {
	class ChessEngine {
		Color m_currCol;

		enum side_t{QUEENSIDE, KINGSIDE};
		//True, when valid, Piece and color check
		bool m_grabCheck(const Position& src) {  
			auto piece = m_Board.getPieceAt(src);
			return ((piece != nullptr) && (piece->getColor() == m_currCol)); //If there is a piece, and it's color is ok
		}
		//Checking for castling, en Passant, or pawn swap
		bool m_castlingCheck(const Position& src, const Position& dest);
		bool m_tryCastle(side_t side, Piece* kingCstl);
		bool m_tryStepExecute(const Position& src, const Position& dest, MovType mvType);
		bool m_tryKnightStep(const Position& src, const Position& dest);
		bool m_enPassantCheck(const Position& src, const Position& dest);
		//Checks whether the way is free to the dest. Does not include destination.
		bool m_isWayFree(const Position& src, const Position& dest);

		const King* m_whiteKing;
		const King* m_blackKing;

		const Position* m_currKingPos;
		void m_setCurrKingPos() {
			if(m_whiteKing != nullptr && m_blackKing != nullptr) {
				m_currKingPos = (m_currCol == Color::WHITE) ?
				&(m_whiteKing->getPosition()) : &(m_blackKing->getPosition());
			}
		}

		class lineITER {
		public:
			enum class iterDIR{UP = 0, DOWN = 1, RIGHT = 2, LEFT=3, UPRIGHT=4, UPLEFT=5, DOWNRIGHT=6, DOWNLEFT=7};
			lineITER(iterDIR iteratorDirection, const Position& start)
				: m_iterDirection(iteratorDirection), m_currPos(start), m_finished(false)
			{}
			bool isFinished() const {return m_finished;	}
			void operator++(int);
			const Position& operator*() const { return m_currPos; }
			const Position& getCurrentPosition() { return m_currPos; }
			iterDIR getIterDirection() const {return m_iterDirection;}
		private:
			const iterDIR m_iterDirection;
			bool m_finished;
			Position m_currPos;
		};
		struct intPair{
			int a;
			int b;
		};
		struct flags{
			flags() : isCheck(false), isMate(false) {}
			bool isCheck;
			bool isMate;
		};

		template <class T>
		bool m_isIntSectEmpty(const vector<T>& inA, vector<T>::iterator bBegin, vector<T>::iterator bEnd) const{
			for (auto it = inA.begin(); it != inA.end(); it++) {
				for (auto itB = bBegin; itB != bEnd; it++) {
					if((*it) == (*itB)){
						return false;
					}
				}
			}
			return true;
		}

		bool m_checkStraightDir(const Position& currPos, int& alliedPieces, const Position& kingPos); //When returns true, the outer loop should be broken.
		void m_checkKnightDir(const Position& kingPos);
		void m_checkEvaluate(const Position& kingPos);
		bool m_checkForMate();
		bool m_canKingAvoidCheck();
		void m_roundEnd();

		PieceType (*m_pawnSwapEvent)(void);

		bool m_activeCheck;
		flags m_flags;

		Pawn* m_delEnPassCache; //EnPass able pawn stored here, EnPass ability deleted in the next round
		Board& m_Board;
	public:
		void setPawnSwapFunction(PieceType (*fnc)(void)){ m_pawnSwapEvent = fnc;}
		const flags& gameFlags;
		ChessEngine(Board& chessBoard, const King* whiteKing = nullptr, const King* blackKing = nullptr);
		void setKing(const King* whiteKing, const King* blackKing){
			m_whiteKing = whiteKing;
			m_blackKing = blackKing;
			m_setCurrKingPos();
		}
		bool ProcessStep(const Position& src, const Position& dst);
		const Color& getCurrentColor() const { return m_currCol;}
		~ChessEngine(void);
	};

}
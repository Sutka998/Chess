#pragma once
#include "Piece.h"
#include "Pawn.h"
#include "Board.h"
#include "King.h"
#include <vector>

namespace ch {

	/**
	* \file ChessEngine.h
	* \brief Header of the ChessEngine
	*/

	/** \brief Manages the game-flow demanding the rules.
	*/
	class ChessEngine : public ISerializable {
		Color m_currCol; /**< \brief The current player's color. */

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

		const King* m_whiteKing;/**< \brief Pointer to the current white king */
		const King* m_blackKing;/**< \brief Pointer to the current black king */

		const Position* m_currKingPos; /**< \brief Pointer to the current king's position */
		void m_setCurrKingPos() {
			if(m_whiteKing != nullptr && m_blackKing != nullptr) {
				m_currKingPos = &((m_currCol == Color::WHITE) ?
					m_whiteKing->getPosition() : m_blackKing->getPosition());
			}
		}
		/** \brief An iterator class, for iterating to specific directions on the board.
		*/
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
		/** \brief A pair struct.
		*/
		struct intPair{
			int a;
			int b;
		};
		/** \brief Struct for the game flags.
		*/
		struct flags{
			flags() : isCheck(false), isMate(false) {}
			bool isCheck;
			bool isMate;
		};

		// 		template <class T>
		// 		bool m_isIntSectEmpty(const std::vector<T>& inA, std::vector<T, std::allocator<T>>::iterator bBegin, std::vector<T, std::allocator<T>>::iterator bEnd) {
		// 			for (auto it = inA.begin(); it != inA.end(); it++) {
		// 				for (auto itB = bBegin; itB != bEnd; it++) {
		// 					if((*it) == (*itB)){
		// 						return false;
		// 					}
		// 				}
		// 			}
		// 			return true;
		// 		}

		bool m_checkStraightDir(const Position& currPos, int& alliedPieces, const Position& kingPos); //When returns true, the outer loop should be broken.
		void m_checkKnightDir(const Position& kingPos);
		void m_checkEvaluate(const Position& kingPos);
		void m_checkForMate() {}
		bool m_canKingAvoidCheck();
		void m_roundEnd();

		PieceType (*m_pawnSwapEvent)(void); /**< \brief A function pointer, called when pawn swap occurs. */

		bool m_activeCheck; /**< \brief A flag, sets by the checkEvaluate function. */
		flags m_flags; /**< \brief Game flags, check and mate state is stored here. */

		Pawn* m_delEnPassCache; /**< \brief An enPassant able pawn is stored here, should be deleted after a round. */
		Board& m_Board;
	public:
		/** \brief Set the given function pointer as pawnSwap event function.
		* \details In case of pawn swap, the engine calls this function, to get the piece that the user wants to replace the pawn.
		*\param [in] A function pointer with PieceType return value, and void argument list.
		*/
		void setPawnSwapFunction(PieceType (*fnc)(void)){ m_pawnSwapEvent = fnc;}
		const flags& gameFlags; /**< \brief  Reference to the game flags, to get them from the outside. */
		ChessEngine(Board& chessBoard, const King* whiteKing = nullptr, const King* blackKing = nullptr);
		/** \brief Setter for the king pointers.
		*/
		void setKing(const King* whiteKing, const King* blackKing){
			m_whiteKing = whiteKing;
			m_blackKing = blackKing;
			m_setCurrKingPos();
		}
		bool ProcessStep(const Position& src, const Position& dst);

		//Serialize
		void Serialize(std::ofstream&);
		void Deserialize(std::ifstream&);
		/** \brief Getter for the current color.
		* \return the current color.
		*/
		const Color& getCurrentColor() const { return m_currCol;}

		void ResetEngine();
		~ChessEngine(void);
	};

}
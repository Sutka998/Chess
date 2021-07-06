#pragma once
#include "position.h"
#include "Iserializable.h"
#include <vector>
#include <fstream>
#include <string>

namespace ch {
	enum class Color {
		BLACK, WHITE
	};
	enum class PieceType {
		PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING
	};
	enum class MovType{
		MOVE, HIT
	};

	inline std::ofstream& operator<<(std::ofstream& os, Color c) {
		if(c == Color::WHITE) 
			os<<"WHITE";
		else
			os<<"BLACK";
		return os;
	}
	inline std::ifstream& operator>>(std::ifstream& is, Color& c) {
		std::string a;
		is>>a;
		if(a == "WHITE") {
			c = Color::WHITE;
		}
		else if(a =="BLACK") {
			c = Color::BLACK;
		}
		else {
			is.setstate(std::ios::failbit);
		}
		return is;
	}
	std::ofstream& operator<<(std::ofstream& os, PieceType pc);
	std::ifstream& operator>>(std::ifstream& is, PieceType& pc);

	class Piece : public ISerializable{
	private:
		//Depends on the type of piece
		virtual void m_evalHitGrid() = 0;
		virtual void m_evalMoveGrid() = 0;
		bool m_isInGrid(MovType, const Position&) const;

		void m_move(const Position&);
		std::vector<Position> m_notCheckGrid;
	protected:
		//Have hitGrid, or moveGrid been evaluated since the last step
		bool m_mGridEvaled; 
		bool m_hGridEvaled;
		//If the hit grid, and move grid are the same, this method creates the hit grid from the move grid
		void m_hGridEqMvGrid();

		bool m_isMoved;/**< \brief Stores whether the piece is moved, or not */
		Position m_position;/**< \brief The current position of the piece */
		const Color m_color; /**< \brief The color of the piece */

		mutable bool m_isCacheValid; /**< \brief Stores if the moveHit cache valid */
		mutable	Position m_canMoveHitCache;/**< \brief Cache for the last position, if the CanMoveHit() returned with true */
		//Movements that are can be done
		//Contains the coordinates, where the piece can step, no repeatings.
		std::vector<Position> m_hitGrid; /**< \brief The positions where the Piece can "hit" */
		std::vector<Position> m_moveGrid;/**< \brief The positions where the Piece can "move" */
	public:
		Piece(Color, const Position&, PieceType);
		virtual ~Piece(void) {}
		const PieceType pieceType; /**< \brief The type of the piece */
		bool canMoveHit(const Position&, MovType) const;
		virtual void Move_Hit(const Position&);

		//Interface
		virtual void Serialize(std::ofstream&) ;
		virtual void Deserialize(std::ifstream&);

		//Getters
		/** \brief Returns the piece's hit grid.
		* \details Hit grid contains the positions, where the piece can "Hit".
		*\return a const reference to the hit grid vector.
		*/
		const std::vector<Position>& getHitGrid() const {return m_hitGrid;}
		/** \brief Returns the piece's move grid.
		* \details Hit grid contains the positions, where the piece can "Move".
		*\return a const reference to the move grid vector.
		*/
		const std::vector<Position>& getMoveGrid() const {return m_moveGrid;}
		/** \brief Getter for the current position.
		*\return a const reference to the current position.
		*/
		const Position& getPosition() const { return m_position;}
		/** \brief Returns if the piece was moved.
		*/
		bool isMoved() const {return m_isMoved;}
		/** \brief Returns the piece's color.
		*/
		Color getColor() const {return m_color;}
	};
}
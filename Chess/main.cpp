#include <SFML/Graphics.hpp>
#include "position.h"

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

namespace ch{
	template <class _FPtr>
	class Event{
		std::list<_FPtr> m_toCall;

	public:
		Event() {}
		template<class _PrA>
		void operator()(_PrA prA){
			for (auto it = m_toCall.begin(); it!=m_toCall.end(); it++){
				(*it)(prA);
			}
		}
		template<class _PrA, class _PrB>
		void operator()(_PrA prA, _PrB prB);
		template<class _PrA, class _PrB, class _PrC>
		void operator()(_PrA prA, _PrB prB, _PrC prC);

		bool isValid() {
			return !m_toCall.empty();
		}
		bool operator!() {
			return isValid();
		}
		void operator+=(_FPtr fvP){
			m_toCall.push_back(fvP);
		}
		void operator-=(_FPtr fvP) {
			for (auto it = m_toCall.begin(); it!= m_toCall.end(); it++) {
				if(*it == fvP) {
					m_toCall.erase(it);
					return;
				}
			}
		}

	};
}
void abc(int a) {
	std::cout<<"Az int: "<< a<<std::endl;
}

int*& test(int** arr){
	return arr[2];

}

using namespace ch;
int main(){	
	/*ch::Event<void(*)(int)> e;
	e+=abc;
	int x = 12;
	e(12);*/
	Graphics graph(std::cout);
	Board brd;
	graph.setBoard(&brd);
	/*for (int i = 0; i < 8; i++) {
		brd.placePieceAt(Color::WHITE, Position(Column(i+1), 2), PieceType::PAWN);
		brd.placePieceAt(Color::BLACK, Position(Column(i+1), 7), PieceType::PAWN);
	}*/
	brd.placePieceAt(Color::BLACK, Position(Column::CL::F, 2), PieceType::PAWN);
	brd.placePieceAt(Color::WHITE, Position(Column::CL::G, 2), PieceType::PAWN);
	brd.placePieceAt(Color::WHITE, Position(Column::CL::E, 1), PieceType::KING);
	brd.placePieceAt(Color::BLACK, Position(Column::CL::E, 8), PieceType::KING);
	graph.renderImg();
	ChessEngine che(brd, *(King*)brd.getPieceAt(Position(Column::CL::E, 1)), *(King*)brd.getPieceAt(Position(Column::CL::E, 8)));
	while (1) {
		std::string in;
		getline(std::cin, in);
		std::istringstream ss(in);
		unsigned fr1, fr2, de1, de2;
		ss>>fr1>>fr2>>de1>>de2;
		if(che.ProcessStep(Position(fr1, fr2), Position(de1, de2))){
			graph.renderImg();
			std::cout<<"Processed\n";
		}
	}
	return 0;
}
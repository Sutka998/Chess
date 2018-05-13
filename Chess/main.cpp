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


void abc(int a) {
	std::cout<<"Az int: "<< a<<std::endl;
}

int*& test(int** arr){
	return arr[2];

}
ch::PieceType xyz() {
	std::cout<<"Pawn swap, piece? \n";
	char c;
	std::cin >> c;
	switch (c)
	{
	case 'r':
		return ch::PieceType::ROOK;
	case 'n':
		return ch::PieceType::KNIGHT;
	case 'b':
		return ch::PieceType::BISHOP;
	default:
		return ch::PieceType::QUEEN;
	}
}

std::ostream& operator<<(std::ostream& os, ch::Color col) {
	if(col == ch::Color::WHITE) {
		os << "WHITE";
	} else{
		os<<"BLACK";
	}
	return os;
}

void testChess() {
	using namespace ch;
	Graphics graph(std::cout);
	Board brd;
	graph.setBoard(&brd);
/*	for (int i = 0; i < 8; i++) {
		brd.placePieceAt(Color::WHITE, Position(Column(i+1), 2), PieceType::PAWN);
		brd.placePieceAt(Color::BLACK, Position(Column(i+1), 7), PieceType::PAWN);
	}*/
	brd.placePieceAt(Color::BLACK, Position(Column::CL::B, 4), PieceType::PAWN);//F7 k, G5 P
	brd.placePieceAt(Color::WHITE, Position(Column::CL::A, 1), PieceType::QUEEN);
	brd.placePieceAt(Color::WHITE, Position(Column::CL::E, 1), PieceType::KING);
	brd.placePieceAt(Color::BLACK, Position(Column::CL::E, 8), PieceType::KING);
	graph.renderImg();
	ChessEngine che(brd, *(King*)brd.getPieceAt(Position(Column::CL::E, 1)), *(King*)brd.getPieceAt(Position(Column::CL::E, 8)));
	che.setPawnSwapFunction(xyz);
	while (1) {
		std::string in;
		getline(std::cin, in);
		std::istringstream ss(in);
		unsigned fr1, fr2, de1, de2;
		ss>>fr1>>fr2>>de1>>de2;
		if(che.ProcessStep(Position(fr1, fr2), Position(de1, de2))){
			graph.renderImg();
			std::cout<<"Processed, next is: "<<che.getCurrentColor()<<"\n";
		}
	}
}


int main(){	
	/*ch::Event<void(*)(int)> e;
	e+=abc;
	int x = 12;
	e(12);*/
	testChess();

	return 0;
}
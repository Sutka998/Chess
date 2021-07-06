#include <SFML/Graphics.hpp>
#include "position.h"

#include "PiecesH.h"
#include "Piece.h"
#include "position.h"
#include <iostream>
#include <list>
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#include <string>
#include <fstream>
#include "Board.h"
#include "ChessEngine.h"
#include "Graphics.h"
#include <sstream>
#include "Game.h"


int main(){	
	{
		ch::Game g;
		g.NewGame();
	}
	_CrtDumpMemoryLeaks(); 
	return 0;
}
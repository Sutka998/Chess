#include "Game.h"

namespace ch {

	/**
	* \file Game.cpp
	* \brief The game class, manages the user in-and output, and the control of the functions.
	*/

	std::ostream& operator<<(std::ostream& os, ch::Color col) {
		if(col == ch::Color::WHITE) {
			os << "WHITE";
		} else{
			os<<"BLACK";
		}
		return os;
	}
	/** \brief Constructor for a game. Creates a game with a board, and with an engine.
	*/
	Game::Game()
		: m_engine(m_board), m_graphics(m_out), m_commandCache(commands::NOPE)
	{
		m_graphics.setBoard(&m_board);
		m_engine.setPawnSwapFunction(m_askSwap);
	}

	std::ostream& Game::m_out = std::cout;

	void Game::m_createBoard(startPos startType) {
		switch (startType)
		{
		case ch::Game::startPos::CUSTOM:
			break;
		case ch::Game::startPos::SAVED:
			break;
		case ch::Game::startPos::BASIC:
			m_genBasicBoard();
			break;
		default:
			break;
		}
	}

	void Game::m_genBasicBoard(){
		for (int i = 0; i < 8; i++) {
			m_board.placePieceAt(Color::WHITE, Position(Column(i+1), 2), PieceType::PAWN);
			m_board.placePieceAt(Color::BLACK, Position(Column(i+1), 7), PieceType::PAWN);
		}
		m_board.placePieceAt(Color::WHITE, Position(Column::CL::E, 1), PieceType::KING);
		m_board.placePieceAt(Color::BLACK, Position(Column::CL::E, 8), PieceType::KING);
		m_engine.setKing((King*)m_board.getPieceAt(Position(Column::CL::E, 1)), (King*)m_board.getPieceAt(Position(Column::CL::E, 8)));

		m_board.placePieceAt(Color::WHITE, Position(Column::CL::A, 1), PieceType::ROOK);
		m_board.placePieceAt(Color::BLACK, Position(Column::CL::A, 8), PieceType::ROOK);

		m_board.placePieceAt(Color::WHITE, Position(Column::CL::B, 1), PieceType::KNIGHT);
		m_board.placePieceAt(Color::BLACK, Position(Column::CL::B, 8), PieceType::KNIGHT);

		m_board.placePieceAt(Color::WHITE, Position(Column::CL::C, 1), PieceType::BISHOP);
		m_board.placePieceAt(Color::BLACK, Position(Column::CL::C, 8), PieceType::BISHOP);

		m_board.placePieceAt(Color::WHITE, Position(Column::CL::D, 1), PieceType::QUEEN);
		m_board.placePieceAt(Color::BLACK, Position(Column::CL::D, 8), PieceType::QUEEN);

		m_board.placePieceAt(Color::WHITE, Position(Column::CL::F, 1), PieceType::BISHOP);
		m_board.placePieceAt(Color::BLACK, Position(Column::CL::F, 8), PieceType::BISHOP);

		m_board.placePieceAt(Color::WHITE, Position(Column::CL::G, 1), PieceType::KNIGHT);
		m_board.placePieceAt(Color::BLACK, Position(Column::CL::G, 8), PieceType::KNIGHT);

		m_board.placePieceAt(Color::WHITE, Position(Column::CL::H, 1), PieceType::ROOK);
		m_board.placePieceAt(Color::BLACK, Position(Column::CL::H, 8), PieceType::ROOK);
	}

	void Game::m_loop() {
		bool exitRq = false;
		std::string msg = "";
		while(!exitRq) {
			m_graphics.renderImg();
			m_out<< msg;
			m_out<<"Current turn: "<<m_engine.getCurrentColor() << std::endl;
			msg = "";
			try
			{
				m_processUserIn();
				m_processCommand();
			}
			catch (std::string str) {
				msg = str;
			}
			catch (std::exception e) {
				msg = e.what();
			}
			catch(...) {
				msg = "Unknown error occurred.\n";
			}
			if(m_commandCache == commands::EXIT || m_commandCache == commands::RESTART){
				break;
			}
		}
	}
	void Game::m_processUserIn() {
		std::string input;
		do{
			std::getline(std::cin, input);
		} while(input == "");
		std::istringstream ss(input);
		std::string str;
		ss>> str;
		if(str == "game") {
			ss>> str;
			m_commandCache = commands::NOPE;
			if (str == "exit") {
				m_commandCache = commands::EXIT;
			}
			else if(str =="restart") {
				m_commandCache = commands::RESTART;
			}
			else if(str == "save") {
				m_commandCache = commands::SAVE;
			}
			else if(str == "load") {
				m_commandCache = commands::LOAD;
			} 
			else {
				std::string excp = std::string("game")+str+std::string("is invalid command, available commands are: game exit, restart, save, load\n");
				throw excp;
			}
		}
		else if(str == "help"){
			throw std::string("game exit, game restart, game save, game load; help; move the piece: X# Y#; X, Y are letters, # are numbers\n");
		}
		else {
			char c, d;
			int i, j;
			Position src, dest;
			ss = std::istringstream(input);
			ss>>c>>i>>d>>j;
			try{
				src = Position(toNum1_8(c), i);
				dest = Position(toNum1_8(d), j);
			}
			catch(std::invalid_argument e) {
				throw std::string("Invalid move command, try it again. X# Y#; X, Y are letters, # are numbers. a-h, 1-8\n");
			}
			m_makeStep(src, dest);
			return;
		}
	}

	void Game::m_makeStep(const Position& src, const Position& dest) {
		if(m_engine.ProcessStep(src, dest)) {
			return;
		}
		throw std::string("Invalid step. Try again.\n");
	}

	void Game::m_processCommand() {
		std::string a;
		try{
			switch (m_commandCache)
			{
			case ch::Game::commands::RESTART:
				return;
			case ch::Game::commands::EXIT:
				return;
			case ch::Game::commands::SAVE:
				m_out<<"Give a filename without extension:\n";
				std::cin>>a;
				if(a == "") {
					throw std::string("Invalid file name\n");
				}
				m_save(a);
				break;
			case ch::Game::commands::LOAD:
				m_out<<"Give a filename without extension:\n";
				std::cin>>a;
				if(a == "") {
					throw std::string("Invalid file name\n");
				}
				m_load(a);
				break;
			default:
				break;
			}
		}
		catch(...) {
			m_commandCache = commands::NOPE;
			throw;
		}
		m_commandCache = commands::NOPE;
	}

	ch::PieceType Game::m_askSwap() {
		while (true) {
			m_out<<"Choose a piece to swap: (q -queen, r -rook, n -knight, b -bishop)\n";
			char c;
			std::cin>>c;
			if(std::cin.good()) {
				switch (tolower(c))
				{
				case 'q':
					return PieceType::QUEEN;
				case 'r':
					return PieceType::ROOK;
				case 'b':
					return PieceType::BISHOP;
				case 'n':
					return PieceType::KNIGHT;
				}
			}
			m_out<<"Wrong character!\t";
			std::cin.clear();
			std::cin.ignore();
		}
	}

	void Game::m_save(std::string filename) {
		std::string FNAME = filename+".che";
		std::ofstream os(FNAME);
		m_board.Serialize(os);
		m_engine.Serialize(os);
		os.close();
	}

	void Game::m_load(std::string filename) {
		std::string FNAME = filename+".che";
		std::ifstream is(FNAME);
		m_board.Deserialize(is);
		Piece *kingW = nullptr, *kingB = nullptr;
		for (auto iter = m_board.it_Begin(Color::WHITE); iter != m_board.it_End(Color::WHITE); iter++) {
			if((*iter)->pieceType == PieceType::KING) {
				kingW = *iter;
				break;
			}
		}
		for (auto iter = m_board.it_Begin(Color::BLACK); iter != m_board.it_End(Color::BLACK); iter++) {
			if((*iter)->pieceType == PieceType::KING) {
				kingB = *iter;
				break;
			}
		}
		m_engine.setKing((King*)kingW, (King*)kingB);
		m_engine.Deserialize(is);
		is.close();
	}
	/** \brief To start a new game. It starts the game loop, and everything for the game. Returns, if the user exited from the game.
	*/
	void Game::NewGame() {
		do{
			m_commandCache = commands::NOPE;
			m_createBoard(startPos::BASIC);
			m_loop();
			m_board = Board();
			m_engine.ResetEngine();
		}while(m_commandCache == commands::RESTART);
	}

	Game::~Game(void)
	{
	}
}
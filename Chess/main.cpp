#include <SFML/Graphics.hpp>
#include "position.h"

#include "Piece.h"
#include "position.h"
#include <iostream>
#include <list>
#include <string>
#include <fstream>

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

	};
}
void abc(int a) {
	std::cout<<"Az int: "<< a<<std::endl;
}

int main(){	
	
	/*ch::Event<void(*)(int)> e;
	e+=abc;
	int x = 12;
	e(12);*/
	getchar();

	return 0;
}
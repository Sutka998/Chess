#include <list>
namespace ch{

	template <class _FPtr>
	class Event{
		std::list<_FPtr> m_toCall;

	public:
		Event() {}


		void operator()(){
			for (auto it = m_toCall.begin(); it!=m_toCall.end(); it++){
				return (*it)();
			}
		}
		
		template<class _PrA>
		void operator()(_PrA prA){
			for (auto it = m_toCall.begin(); it!=m_toCall.end(); it++){
				(*it)(prA);
			}
		}

		template<class _PrA, class _PrB>
		void operator()(_PrA prA, _PrB prB) {
			for (auto it = m_toCall.begin(); it!=m_toCall.end(); it++){
				(*it)(prA, prB);
			}
		}

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
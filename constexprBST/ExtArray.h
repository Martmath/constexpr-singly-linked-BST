#pragma once

#include "TreeWrap.h"
#include <iostream>



template<class T, size_t N, template <class, auto> class C>
auto ArrtoStr(C<T,N> mA)
{

	ostringstream ss;
	for (size_t i = 0; i < N; i++)	ss << mA[i]<< ' ';
	ss << "\n";
	return ss.str();
}





template<class T, size_t N>
struct _array :array<T, N>
{
	constexpr	_array(array<T, N> a) :array<T, N>(a)	{	}
	constexpr _array() {	}

	//constexpr _array(initializer_list<T> a) :array<T, N>{a} { }

	template <typename... W> constexpr
		_array(W... ts) : array<T, N>{ ts... } { 	}
	template<class T, size_t N>
	friend ostream& operator<<(ostream& os, const _array<T,N>& dt);
};
template<class T, size_t N>
ostream& operator<<(ostream& os, const _array<T, N>& dt)
{
	os << ArrtoStr(dt);
	return os;
}


template<const auto& T0, const auto &... T> struct arrPack
{	
	static constexpr array<Test, sizeof...(T) + 1> WW{ T0,T... };
	
	static constexpr auto tmpArr() {
		return array<Test, sizeof...(T) + 1>{ T0,T... };	}	

	template<const auto & F> static constexpr auto TrWrap() {
		return TreeWrap<F, WW, varCreate::autoAlgCreate,3, getTInputTemplate<clearType_t<decltype(WW)>>>();
	}
};

static constexpr Test E0{ 7 }; static constexpr Test E1{ 12 }; static constexpr Test E2{ 15 };
static constexpr Test E3{ 5 }; static constexpr Test E4{ 3 }; static constexpr Test E5{ 0 };

static constexpr arrPack<E0, E1, E2, E3, E4, E5> checkArrWrp{};
static constexpr auto packArr = decltype(checkArrWrp)::tmpArr();
constexpr auto vv1 = decltype(checkArrWrp)::TrWrap<getKeyFn>();
using VV1 = decltype(vv1);
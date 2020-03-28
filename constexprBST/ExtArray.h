#pragma once

//#include "TreeWrap.h"
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



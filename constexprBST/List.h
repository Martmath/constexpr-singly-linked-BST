#pragma once
#include "OutSupport.h"
template<int X, class H, class T> struct CList
{
	const H head{}; const T tail{ }; const int uid = 0;
};
template<int X, class H> struct CList<X, H, CZero> {
	const H head{}; const int uid = 0;
};
template<> struct CList<0, CZero, CZero> {};
using CListLastZero = CList<0, CZero, CZero>;
template<class F> using CListLast = CList<1, F, CZero>;

template<auto F, class T, int N, template <class, auto> class TA, const TA<T, N> & Arr, size_t L, size_t Mmax, const size_t d0>
constexpr auto createListOld() {
	return CListLast<T>{ Arr[d0], d0 };
}

template<auto F, class T, int N, template <class, auto> class TA,const TA<T, N> & Arr, size_t L, size_t Mmax,
	const size_t d0, const size_t d1, const size_t... d>
	constexpr auto createListOld() {
	if constexpr ((sizeof...(d) == Mmax))//|| (F(Arr[d1]) == F(T{})))
	{
		if constexpr (F(Arr[d0]) == F(Arr[d1]))

		{
			constexpr auto tail_ = CListLast<T>{ Arr[d1],d1 };
			return CList<L, T, decltype(tail_)> {Arr[d0], tail_, d0 };
		}
		else
			return CListLast<T>{ Arr[d0], d0};
	}
	else
	{
		if constexpr (F(Arr[d0]) == F(Arr[d1]))
		{
			constexpr auto tail_ = createListOld<F, T, N,TA, Arr, L + 1, Mmax, d0, d...>();
			return CList<L, T, decltype(tail_)> {Arr[d1], tail_, d1 };
		}
		else
		{
			return createListOld<F, T, N,TA, Arr, L + 1, Mmax, d0, d...>();
			//return CListLast<T>{ Arr[d0], d0};
		}
	}
}

template<auto F, class Q, int N, template <class, auto> class TA, const TA<Q, N> & Arr, size_t L, const size_t d0>
constexpr auto createList() {
	return CListLast<Q>{ Arr[d0], 0 /*d0*/	 };
}
template<auto F, class Q, int N, template <class, auto> class TA, const TA<Q, N> & Arr, size_t L,
	const size_t d0, const size_t d1, const size_t... d>
	constexpr auto createList() {
	if constexpr (sizeof...(d) == 0)
	{
		if constexpr (F(Arr[d0]) == F(Arr[d1]))

		{
			constexpr auto tail_ = CListLast<Q>{ Arr[d1],  0 };
			return CList<L, Q, decltype(tail_)> {Arr[d0], tail_, 1 };
		}
		else
			return CListLast<Q>{ Arr[d0], 1};
	}
	else
	{
		if constexpr (F(Arr[d0]) == F(Arr[d1]))
		{
			constexpr auto tail_ = createList<F, Q, N,TA, Arr, L + 1, d1, d...>();
			return CList<L, Q, decltype(tail_)> {Arr[d0], tail_, sizeof...(d) + 1 };
		}
		else
		{
			return CListLast<Q>{ Arr[d0], sizeof...(d) + 1};
		}
	}
}


template<auto F, class T, int N, const array<T, N> & Arr, size_t L = 0, const size_t d0>
constexpr auto createListtoArray() {
	if constexpr (F(Arr[d0]))
	{
		return CListLast<T>{ Arr[d0], d0 };
	}
	else return  CListLastZero{};
}

template<auto F, class T, int N, const array<T, N> & Arr, size_t L = 0,
	const size_t d0, const size_t d1, const size_t... d>
	constexpr auto createListtoArray() {
	if constexpr (sizeof...(d) == 0)
	{
		if constexpr (F(Arr[d0]))

		{
			constexpr auto tail_ = CListLast<T>{ Arr[d1],d1 };
			return CList<L, T, decltype(tail_)> {Arr[d0], tail_, d0 };
		}
		else
			return CListLastZero{};
	}
	else
	{
		if constexpr (F(Arr[d0]))
		{
			constexpr auto tail_ = createListtoArray<F, T, N, Arr, L + 1, d1, d...>();
			return CList<L, T, decltype(tail_)> {Arr[d0], tail_, d0 };
		}
		else
		{
			return CListLastZero{};
		}
	}
}

template<auto F, class T, int N, const array<T, N> & Arr, size_t L,
const size_t... d> constexpr auto fromArraytocList(std::index_sequence<d...> DD)
{
	return createListtoArray<F, T, N, Arr, L + 1, d...>();
}

//intellisense -work
template<auto F, class T, int N, const array<T, N> & Arr> constexpr auto fromArraytocList()
{
	return fromArraytocList<F, T, N, Arr, 0>(std::make_index_sequence<N>{});
}

//intellisense -not work
template<auto F, const auto & Arr> constexpr auto fromArraytocList()
{
	using T = getTInputTemplate<decltype(Arr)>;
	return fromArraytocList<F, typename T::Tin, T::Size, Arr, 0>(std::make_index_sequence<T::Size>{});
}

enum  ListType { ltZero = 0, ltLast, ltSimple, ltOther };
template <class T>
struct isLT { constexpr static ListType it() { return ltOther; }; };

template <int I, class InClass, class Tail >
struct isLT<CList<I, InClass, Tail>>
{
	constexpr static ListType it()
	{
		if (sameZero<Tail>() && (I == 0)) return ltZero;
		else return (sameZero<Tail>()) ? ltLast : ltSimple;
	}
	constexpr static bool isltZero() { return sameZero<Tail>() && (I == 0); };
	constexpr static bool isltLast() { return sameZero<Tail>() && (I != 0); };
};

template < typename R> constexpr size_t listCount(R t)
{
	if constexpr (isLT <R>::it() == ltLast)
	{
		return 1;
	}
	else
	{
		if constexpr (isLT <R>::it() != ltZero)
		{
			return 1 + listCount(t.tail);
		}
		else
			return 0;
	}
}
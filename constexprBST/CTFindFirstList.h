
#pragma once
#include <iostream>
#include <array>
#include "TreeCreatorOld.h"
#include "TreeCreator.h"
#include "CTHelper.h"

template <typename F, typename T, size_t N, template <class, auto> class TA, typename R>
constexpr void findFirstListHelperArray(F f, TA<T, N>& mA, R t, FuncRes<F> c)
{
	if constexpr (isTN <R>::istnLast())
	{
		if (f(t.list.head) == c) return fillArrayFromList(mA, t.list);
	}
	else
	{
		if constexpr (!isTN <R>::istnZero())
		{
			if (f(t.list.head) == c) fillArrayFromList(mA, t.list);
			else if (f(t.list.head) < c) return findFirstListHelperArray(f,mA,t.right, c);
			else return findFirstListHelperArray(f,mA,t.left, c);
		}
	}
}

template <class T, template <class, auto> class TA, size_t N = BASENUM, typename F, typename R>
constexpr auto findFirstListArray(F f, R t,  T c) noexcept
{
	using TP = getTInputTemplate<R>;
	using B = TreeBase<R>;
	if constexpr ((hasSizeResT<TP>{}) && (N == BASENUM))
	{
		auto tempArray = fillArrayBaseV<TA, B>(make_index_sequence<TP::Size>{});
		size_t count = 0;
		findFirstListHelperArray(f, tempArray, t, c);
		packArray(f, tempArray, &count);
		return tempArray;
	}
	else
	{
		auto tempArray = fillArrayBaseV<TA, B>(make_index_sequence<N>{});
		size_t count = 0;
		findFirstListHelperArray(f, tempArray, t, c);
		packArray(f, tempArray, &count);
		return tempArray;
	}
}

template <const auto & F, const auto & Tree,
FuncRes<decltype(F)> C, template <class, auto> class TA = array>
constexpr auto findFirstListA()
{
	return TempWrapperArgs1<FuncRes<decltype(F)>, TA, F, Tree, findFirstListArray<FuncRes<decltype(F)>, TA>, C>{}();
}

//variant with array
template <class T, const auto & F, const auto & Tree, T C, template <class, auto> class TA = array>
constexpr auto findFirstListA()
{
	return TempWrapperArgs1<T, TA, F, Tree, findFirstListArray<T, TA>, C>{}();
}

#define FINDFIRSTLISTA(F,Tree,It,...) \
findFirstListA<typename getTInputTemplate<decltype(F)>::Tres,F,Tree,It,__VA_ARGS__>()

template<auto F, typename R>
constexpr size_t findFirstListCount(R t, FuncRes<decltype(F)> c)
{
	if constexpr (isTN<R>::istnZero())  return  0;
	else
	{
		if constexpr (isTN<R>::istnLast())
		{
			if (F(t.list.head) == c) return listCount(t.list); else return 0;
		}
		else
		{
			if (F(t.list.head) == c) return listCount(t.list);
			else if (F(t.list.head) < c) return findFirstListCount<F>(t.right, c);
			else return findFirstListCount<F>(t.left, c);
		}
	}
}

template<auto F, typename T = FuncIn<decltype(F)>, typename R>
constexpr auto findFirstListItem(R t, size_t N, FuncRes<decltype(F)> c)
{
	if constexpr (isTN<R>::istnZero()) 	return T{};
	else
	{
		if constexpr (isTN<R>::istnLast())
		{
			if (F(t.list.head) == c) return getListItem(t.list, N, 0);
			else return T{};
		}
		else
		{
			if (F(t.list.head) == c) return getListItem(t.list, N, 0);
			else if (F(t.list.head) < c) return findFirstListItem<F>(t.right, N, c);
			else return findFirstListItem<F>(t.left, N, c);
		}
	}
}

template<auto F, template <class, auto> class TA, class T, std::size_t... I>
constexpr auto findFirstListHelper(T W, FuncRes<decltype(F)> c, std::index_sequence<I...>)
{
	return TA<TreeBase<T>, sizeof...(I)>{findFirstListItem<F>(W, I, c)... };
}

template<auto F, const auto & T, FuncRes<decltype(F)> c, template <class, auto> class TA = array>
constexpr auto findFirstList()
{
	constexpr size_t  Q = findFirstListCount<F>(T, c);
	return  findFirstListHelper<F, TA>(T, c, std::make_index_sequence<Q>{});
}

#define FINDFIRSTLIST(F,Tree,c,...) \
findFirstList<F,Tree,c,__VA_ARGS__>()
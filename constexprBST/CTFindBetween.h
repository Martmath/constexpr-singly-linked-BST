#pragma once
#include <iostream>
#include <array>
#include "TreeCreatorOld.h"
#include "TreeCreator.h"
#include "CTHelper.h"

template <typename F, typename T, size_t N, template <class, auto> class TA, typename R>
constexpr void findBetweenHelperArray(F f, TA<T, N>& mA, R t, 
 FuncRes<F> l, FuncRes<F> r)
{
	if constexpr (isTN<R>::istnLast())
	{
		if ((f(t.list.head) >= l) && (f(t.list.head) <= r)) 
			fillArrayFromList(mA, t.list);
	}
	else
	{
		if constexpr (!isTN <R>::istnZero())
		{
			if ((f(t.list.head) >= l) && (f(t.list.head) <= r))
			{
				fillArrayFromList(mA, t.list);
				findBetweenHelperArray(f, mA, t.left, l, r);
				findBetweenHelperArray(f, mA, t.right, l, r);
			}
			else if ((f(t.list.head) <= l))
			{
				findBetweenHelperArray(f, mA, t.right, l, r);
			}
			else if ((f(t.list.head) >= r))
			{
				findBetweenHelperArray(f, mA, t.left, l, r);
			}
		}
	}
}

template <class T, template <class, auto> class TA = array, size_t N = BASENUM,  typename F, typename R>
constexpr auto findBetweenArray(F f, R t, T l, T r) 
{	
	using TP = getTInputTemplate<R>;
	using B = TreeBase<R>;
	if constexpr ((hasSizeResT<TP>{}) && (N == BASENUM))
	{
		auto tempArray = fillArrayBaseV<TA,B>(make_index_sequence<TP::Size>{});
		size_t count = 0;
		findBetweenHelperArray(f, tempArray, t, l, r);
		packArray(f, tempArray, &count);
		return tempArray;
	}
	else
	{
		auto tempArray = fillArrayBaseV<TA, B>(make_index_sequence<N>{});
		size_t count = 0;
		findBetweenHelperArray(f, tempArray, t, l, r);
		packArray(f, tempArray, &count);
		return tempArray;
	}
}
//class T, template <class, auto> class TA, const auto & F, const auto & Tree,T L,T R,
template <const auto& F,const auto & Tree, 
FuncRes<decltype(F)> L, FuncRes<decltype(F)>  R, template <class, auto> class TA = array>
constexpr auto findBetweenA()
{
	return TempWrapperArgs2<FuncRes<decltype(F)>,TA,F,Tree, findBetweenArray<FuncRes<decltype(F)>,TA>, L, R>{}();
}

//variant with array
template <class T, const auto & F, const auto & Tree, T  L, T  R, template <class, auto> class TA=array>
constexpr auto findBetweenA()
{
	return TempWrapperArgs2<T,TA, F, Tree, findBetweenArray<T,TA>, L, R>{}();
}

#define FINDBETWEENA(F,Tree,Beg,End,...) \
findBetweenA<typename getTInputTemplate<decltype(F)>::Tres,F,Tree,Beg,End,__VA_ARGS__>()

template <auto F, typename R>
constexpr size_t findBetweenCount(R t, FuncRes<decltype(F)> l, FuncRes<decltype(F)> r)
{
	if constexpr (isTN<R>::istnLast())
	{
		if ((F(t.list.head) >= l) && (F(t.list.head) <= r))
		{
			return listCount(t.list);
		}
		else { return 0; }
	}
	else
	{
		if constexpr (!isTN <R>::istnZero())
		{
			if ((F(t.list.head) >= l) && (F(t.list.head) <= r))
			{
				return  listCount(t.list) + 
				findBetweenCount<F>(t.left, l, r) + findBetweenCount<F>(t.right, l, r);
				
			}
			else if ((F(t.list.head) <= l))
			{
				return  findBetweenCount<F>(t.right, l, r);
			}
			else if ((F(t.list.head) >= r))
			{
				return  findBetweenCount<F>(t.left, l, r);
			}
		}
		else return 0;
	}
}

template <auto F, typename R>
constexpr bool findBetweenItemBool(R t, size_t N, FuncRes<decltype(F)> l, FuncRes<decltype(F)> r)
{
	if constexpr (isTN<R>::istnLast())
	{
		if ((F(t.list.head) >= l) && (F(t.list.head) <= r))
		{
			if (listCount(t.list) >= (N + 1))
				return true;
			else
				return false;
		}
		else return false;
	}
	else
	{
		if constexpr (!isTN <R>::istnZero())
		{
			if ((F(t.list.head) >= l) && (F(t.list.head) <= r))
			{
				if (listCount(t.list) >= (N + 1))	return true;
				else
				{
					if (findBetweenItemBool<F>(t.left, N - listCount(t.list), l, r))
						return	true;
					else
						return	findBetweenItemBool<F>(t.right,
							N - findBetweenCount<F>(t.left, l, r) - listCount(t.list), l, r);
				}
			}
			else if ((F(t.list.head) <= l))
			{
				return  findBetweenItemBool<F>(t.right, N, l, r);
			}
			else if ((F(t.list.head) >= r))
			{
				return  findBetweenItemBool<F>(t.left, N, l, r);
			}
		}
		else return false;
	}
}

template <auto F, typename T = FuncIn<decltype(F)>, typename R>
constexpr auto findBetweenItem(R t, size_t N, FuncRes<decltype(F)> l, FuncRes<decltype(F)> r)
{
	if constexpr (isTN<R>::istnLast())
	{
		if ((F(t.list.head) >= l) && (F(t.list.head) <= r))
		{
			if (listCount(t.list) >= (N + 1))
				return getListItem(t.list, N, 0);
			else
				return T{};
		}
		else return T{};
	}
	else
	{
		if constexpr (!isTN <R>::istnZero())
		{
			if ((F(t.list.head) >= l) && (F(t.list.head) <= r))
			{
				if (listCount(t.list) >= (N + 1))
				{
					return getListItem(t.list, N, 0);
				}
				else
				{
					if (findBetweenItemBool<F>(t.left, N - listCount(t.list), l, r))
					{
						return	findBetweenItem<F,T>(t.left, N - listCount(t.list), l, r);
					}
					else
						return	findBetweenItem<F,T>(t.right, N - findBetweenCount<F>(t.left, l, r) - listCount(t.list), l, r);
				}
			}
			else if ((F(t.list.head) <= l))
			{
				return  findBetweenItem<F,T>(t.right, N, l, r);
			}
			else if ((F(t.list.head) >= r))
			{
				return  findBetweenItem<F,T>(t.left, N, l, r);
			}
		}
		else return T{};
	}
}

template<auto F,template <class, auto> class TA, class T, std::size_t... I>
constexpr auto findBetweenHelper(T W, FuncRes<decltype(F)> l, FuncRes<decltype(F)> r, std::index_sequence<I...>)
{
	return TA<TreeBase<T>, sizeof...(I)>{findBetweenItem<F>(W, I, l,  r)... };
}

template<auto F, const auto & T, FuncRes<decltype(F)> l, FuncRes<decltype(F)> r, template <class, auto> class TA = array>
constexpr auto findBetween()
{
	constexpr size_t  Q = findBetweenCount<F>(T, l,  r);
	return  findBetweenHelper<F,TA>(T, l, r, std::make_index_sequence<Q>{});
}

#define FINDBETWEEN(F,Tree,Beg,End,...) \
findBetween<F,Tree,Beg,End,__VA_ARGS__>()

#pragma once
#include <iostream>
#include <array>
#include "TreeCreatorOld.h"
#include "TreeCreator.h"
#include "CTHelper.h"
#include "ZeroAll.h"

template <bool Run = false, typename F, typename T, size_t N, template <class, auto> class TA, typename R>
	constexpr void findCurrentNodeHelperArray(F f, TA<T, N>& mA, R t, FuncRes<F> c)
{
	if constexpr (isTN <R>::it() == tnLast)
	{
		if (Run || (f(t.list.head) == c))  fillArrayFromList(mA, t.list);
	}
	else
	{
		if constexpr (isTN <R>::it() != tnZero)
		{
			if (Run || (f(t.list.head) == c))
			{
				fillArrayFromList(mA, t.list);
				findCurrentNodeHelperArray<true>(f, mA, t.left, c);
				findCurrentNodeHelperArray<true>(f, mA, t.right, c);
			}
			else if ((f(t.list.head) <= c))
			{
				findCurrentNodeHelperArray(f, mA, t.right, c);
			}
			else if ((f(t.list.head) >= c))
			{
				findCurrentNodeHelperArray(f, mA, t.left, c);
			}
		}
	}
}

template <class T, template <class, auto> class TA, size_t N = BASENUM, typename F, typename R>
constexpr auto findCurrentNodeArray(F f, R t, T c) noexcept
{
	using TP = getTInputTemplate<R>;
	using B = TreeBase<R>;
	if constexpr ((hasSizeResT<TP>{}) && (N == BASENUM))
	{
		auto tempArray = fillArrayBaseV<TA, B>(make_index_sequence<TP::Size>{});
		size_t count = 0;
		findCurrentNodeHelperArray(f, tempArray, t, c);
		packArray(f, tempArray, &count);
		return tempArray;
	}
	else
	{
		auto tempArray = fillArrayBaseV<TA, B>(make_index_sequence<N>{});
		size_t count = 0;
		findCurrentNodeHelperArray(f, tempArray, t, c);
		packArray(f, tempArray, &count);
		return tempArray;
	}
}

template <const auto & F, const auto & Tree,
	FuncRes<decltype(F)>  C, template <class, auto> class TA = array>
	constexpr auto findCurrentNodeA()
{
	return TempWrapperArgs1<FuncRes<decltype(F)>, TA, F, Tree, findCurrentNodeArray<FuncRes<decltype(F)>, TA>, C>{}();
}

//variant with array
template <class T, const auto & F, const auto & Tree, T C, template <class, auto> class TA = array>
constexpr auto findCurrentNodeA()
{
	return TempWrapperArgs1<T, TA, F, Tree,findCurrentNodeArray<T, TA>,C>{}();
}

#define FINDCURRENTNODEA(F,Tree,C,...) \
findCurrentNodeA<typename getTInputTemplate<decltype(F)>::Tres,F,Tree,C,__VA_ARGS__>()

template <auto F, bool Run = false, typename R>
constexpr size_t findCurrentNodeCount(R t, FuncRes<decltype(F)> c)
{
	if constexpr (isTN<R>::istnLast())
	{
		if (Run || (F(t.list.head) == c))  return listCount(t.list);
	}
	else
	{
		if constexpr (!isTN <R>::istnZero())
		{
			if (Run || (F(t.list.head) == c))
			{
				return listCount(t.list) +
					findCurrentNodeCount<F, true>(t.left, c) + findCurrentNodeCount<F, true>(t.right, c);
			}
			else if ((F(t.list.head) <= c))
			{
				return findCurrentNodeCount<F>(t.right, c);
			}
			else if ((F(t.list.head) >= c))
			{
				return findCurrentNodeCount<F>(t.left, c);
			}
			else
				return 0;
		}
		else return 0;
	}
}

template <auto F, bool Run = false, typename R>
constexpr auto findCurrentNodeItemBool(R t, size_t N, FuncRes<decltype(F)> c)
{
	if constexpr (isTN<R>::istnLast())
	{
		if (Run || (F(t.list.head) == c))
		{
			if (listCount(t.list) >= (N + 1))
				return true;
			else
				return false;
		}
		else 
			return false;
	}
	else
	{
		if constexpr (!isTN <R>::istnZero())
		{
			if (Run || (F(t.list.head) == c))
			{
				if (listCount(t.list) >= (N + 1))	return true;
				else
				{
					if (findCurrentNodeItemBool<F, true>(t.left, N - listCount(t.list), c))
						return  true;
					else
						return	findCurrentNodeItemBool<F, true>(t.right, N - findNodeCount(t.left) - listCount(t.list), c);

				}
			}
			else if ((F(t.list.head) <= c))
			{

				return findCurrentNodeItemBool<F>(t.right, N, c);
			}
			else if ((F(t.list.head) >= c))
			{
				return findCurrentNodeItemBool<F>(t.left, N, c);
			}
			else 
				return false;
		}
		else 
			return false;
	}
}

template <auto F, bool Run = false, typename T = typename getTInputTemplate<decltype (F)>::Tin, typename R>
constexpr auto findCurrentNodeItem(R t, size_t N, FuncRes<decltype(F)> c)
{
	if constexpr (isTN<R>::istnLast())
	{
		if (Run || (F(t.list.head) == c))
		{
			if (listCount(t.list) >= (N + 1))
				return getListItem(t.list, N, 0);
			else
				return Zero<T>();
		}
		else 
			return Zero<T>();	
	}
	else
	{
		if constexpr (!isTN <R>::istnZero())
		{
			if (Run || (F(t.list.head) == c))
			{
				if (listCount(t.list) >= (N + 1))
				{
					return getListItem(t.list, N, 0);
				}
				else
				{
					if (findCurrentNodeItemBool<F, true>(t.left, N - listCount(t.list), c))
					{
						return  findCurrentNodeItem<F, true,T>(t.left, N - listCount(t.list), c);
					}
					else
					{					
						return	findCurrentNodeItem<F, true,T>(t.right, N - findNodeCount(t.left) - listCount(t.list), c);
					}
				}
			}
			else if ((F(t.list.head) <= c))
			{
				return findCurrentNodeItem<F,false,T>(t.right, N, c);
			}
			else if ((F(t.list.head) >= c))
			{
				return findCurrentNodeItem<F,false,T>(t.left, N, c);
			}
			else
				return Zero<T>();
		}
		else return Zero<T>();
	}
}

template<auto F, template <class, auto> class TA, class T, std::size_t... I>
constexpr auto findCurrentNodeHelper(T W, FuncRes<decltype(F)> c, std::index_sequence<I...>)
{
	return TA<TreeBase<T>, sizeof...(I)>{findCurrentNodeItem<F,false,TreeBase<T>>(W, I, c)... };
}

template<auto F, const auto & T, FuncRes<decltype(F)> C, template <class, auto> class TA = array>
constexpr auto findCurrentNode()
{
	constexpr size_t Q = findCurrentNodeCount<F>(T, C);
	return  findCurrentNodeHelper<F, TA>(T, C, std::make_index_sequence<Q>{});
}

#define FINDCURRENTNODE(F,Tree,C,...) \
findCurrentNode<F,Tree,C,__VA_ARGS__>()
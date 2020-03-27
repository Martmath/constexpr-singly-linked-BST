#pragma once
#include <iostream>
#include <array>
#include "TreeCreatorOld.h"
#include "TreeCreator.h"
#include "CTHelper.h"

template <template <class, auto> class TA, const auto & F, const auto & FBool, const auto & Tree,
	TA<TreeBase<decltype(Tree)>, getTInputTemplate<decltype(Tree)>::Size>
	(*DO)(decltype(F),decltype(FBool), decltype(Tree))>
	struct TempWrapperfindAll {
	static constexpr auto Temp = DO(F, FBool, Tree);
	static constexpr auto Res = cutSortedArray
		<F,TA, Temp.size(), TreeBase<decltype(Tree)>, Temp>();
	constexpr auto operator () () const { return Res; };
};

//use template instread array - for your "array" type
template <typename F,typename T, size_t N, template <class, auto> class TA,	typename R>
	constexpr void findAllHelperA(F f, TA<T, N>& mA, R t)
{
	if constexpr (isTN <R>::istnLast())
	{
		if (f(t.list.head))	fillArrayFromList(mA, t.list);
	}
	else
	{
		if constexpr (!isTN <R>::istnZero())
		{
			if (f(t.list.head)) fillArrayFromList(mA, t.list);
			findAllHelperA(f, mA, t.left);
			findAllHelperA(f, mA, t.right);
		}
	}
}

template <template <class, auto> class TA = array, size_t N = BASENUM, typename F, typename FBool, typename R>
constexpr auto findAllArray(F c, FBool f, R t) noexcept
{
	using TP = getTInputTemplate<R>;
	using B = TreeBase<R>;//typename getTInputTemplate <TP::D>::H;
	//constexpr size_t a=((hasSizeResT<TP>{})&&(N==BASENUM))?TP::Size:N;
	if constexpr ((hasSizeResT<TP>{}) && (N == BASENUM))
	{
		auto tempArray = fillArrayBaseV<TA, B>(make_index_sequence<TP::Size>{});
		size_t count = 0;
		findAllHelperA(f, tempArray, t);
		packArray(c, tempArray, &count);
		return tempArray;//std::pair(sorted, NN);
	}
	else
	{
		auto tempArray = fillArrayBaseV<TA, B>(make_index_sequence<N>{});
		size_t count = 0;
		findAllHelperA(f, tempArray, t);
		packArray(c, tempArray, &count);
		return tempArray;//std::pair(sorted, NN);
	}
}
template <const auto & T, const auto & TBool, const auto & Tree, template <class, auto> class TA=array>
constexpr auto findAllA() 
{
	return TempWrapperfindAll<TA, T, TBool, Tree, findAllArray<TA>>{}();
}

template <auto F, typename R> constexpr size_t findAllCount(R t) {
	if constexpr (isTN <R>::it() == tnLast)
		if (F(t.list.head))  return listCount(t.list); else return 0;
	else
		if constexpr (isTN <R>::it() != tnZero)
			if (F(t.list.head))
				return  listCount(t.list) + 
				findAllCount<F>(t.left) + findAllCount<F>(t.right);
			else
				return  findAllCount<F>(t.left) + findAllCount<F>(t.right);
		else return 0;
}

template <auto F, typename R> 
constexpr auto findAllItemBool(R t, size_t N)
{
	if constexpr (isTN <R>::it() == tnLast)
	{
		if (F(t.list.head))
		{
			if (listCount(t.list) >= (N + 1)) return true;
			else                              return false;
		}
		else  return false; 
	}
	else
	{
		if constexpr (isTN <R>::it() != tnZero)
		{
			if (F(t.list.head))
			{
				if (listCount(t.list) >= (N + 1))	return true;
				else
				{
					if (findAllItemBool<F>(t.left, N - listCount(t.list)))	
						return	true;
					else
						return	findAllItemBool<F>(t.right, 
							N - findAllCount<F>(t.left) - listCount(t.list));
				}
			}
			else
			{
				if (findAllItemBool<F>(t.left, N))	return	true;
				else	return	findAllItemBool<F>(t.right, N - findAllCount<F>(t.left));
			}
		}
		else  return false; 
	}
}

template <auto F, typename T = typename getTInputTemplate<decltype (F)>::Tin, typename R>
constexpr auto findAllItem(R t, size_t N)
{
	if constexpr (isTN <R>::it() == tnLast)
	{
		if (F(t.list.head))
		{
			if (listCount(t.list) >= (N + 1)) return getListItem(t.list, N, 0);
			else                              return T{};
		}
		else return T{};
	}
	else
	{
		if constexpr (isTN <R>::it() != tnZero)
		{
			if (F(t.list.head))
			{
				if (listCount(t.list) >= (N + 1))
				{
					return getListItem(t.list, N, 0);
				}
				else
				{
					if (findAllItemBool<F>(t.left, N - listCount(t.list)))
						return	findAllItem<F, T>(t.left, N - listCount(t.list));
					else
						return	findAllItem<F, T>(t.right, 
							N - findAllCount<F>(t.left) - listCount(t.list));
				}
			}
			else
			{
				if (findAllItemBool<F>(t.left, N))
					return	findAllItem<F, T>(t.left, N);
				else
					return	findAllItem<F, T>(t.right, N - findAllCount<F>(t.left));
			}
		}
		else return T{}; 
	}
}

template<auto F, template <class, auto> class TA, class T, std::size_t... I>
constexpr auto findAllHelper(T W, std::index_sequence<I...>)
{
	return TA<TreeBase<T>, sizeof...(I)>{findAllItem<F>(W, I)... };
}

template<auto F, const auto & T, template <class, auto> class TA = array>
constexpr auto findAll()
{
	constexpr size_t  Q = findAllCount<F>(T);
	return findAllHelper<F,TA>(T, std::make_index_sequence<Q>{}); 
}

//#define FINDALL(Func,Tree) \
//findBetween<typename getTInputTemplate<decltype(Func)>::Tres,Func,Tree,Beg,End>()
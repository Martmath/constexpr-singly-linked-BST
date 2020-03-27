#pragma once
#include <iostream>
#include <array>
#include "TreeCreatorOld.h"
#include "TreeCreator.h"
#include "CTHelper.h"


template <typename T, size_t N, template <class, auto> class TA, typename R>
constexpr void findLevelHelperArray(TA<T, N>& mA, R t, size_t FL, size_t CL = 0)
{
	if constexpr (isTN <R>::it() == tnLast)
	{
		if (FL == CL) fillArrayFromList(mA, t.list);
	}
	else
	{
		if constexpr (isTN <R>::it() != tnZero)
		{
			if (FL == CL)
			{
				fillArrayFromList(mA, t.list);
			}
			else
			{
				findLevelHelperArray(mA, t.right, FL, CL + 1);
				findLevelHelperArray(mA, t.left, FL, CL + 1);
			}
		}
	}
}

template <template <class, auto> class TA, size_t N = BASENUM, typename F, typename R>
constexpr auto findLevelArray(F f, R t,  size_t FL) noexcept
{
	using TP = getTInputTemplate<R>;
	using B = TreeBase<R>;
	if constexpr ((hasSizeResT<TP>{}) && (N == BASENUM))
	{
		auto tempArray = fillArrayBaseV<TA, B>(make_index_sequence<TP::Size>{});
		size_t count = 0;
		findLevelHelperArray(tempArray, t, FL);
		packArray(f, tempArray, &count);
		return tempArray;
	}
	else
	{
		auto tempArray = fillArrayBaseV<TA, B>(make_index_sequence<N>{});
		size_t count = 0;
		findLevelHelperArray(tempArray, t, FL);
		packArray(f, tempArray, &count);
		return tempArray;
	}
}

template <typename T, size_t N, template <class, auto> class TA, typename R>
constexpr void findFirstLevelHelperArray(TA<T, N>& mA, R t, size_t FL, size_t CL = 0)
{
	if constexpr (isTN <R>::it() == tnLast)
	{
		if (FL == CL)  fillFirstArrayFromList(mA, t.list);
	}
	else
	{
		if constexpr (isTN <R>::it() != tnZero)
		{
			if (FL == CL)
			{
				fillFirstArrayFromList(mA, t.list);
			}
			else
			{
				findFirstLevelHelperArray(mA, t.right, FL, CL + 1);
				findFirstLevelHelperArray(mA, t.left, FL, CL + 1);
			}
		}
	}
}

template <template <class, auto> class TA, size_t N = BASENUM, typename F, typename R>
constexpr auto findFirstLevelArray(F f, R t, size_t FL) noexcept
{
	using TP = getTInputTemplate<R>;
	using B = TreeBase<R>;
	if constexpr ((hasSizeResT<TP>{}) && (N == BASENUM))
	{
		auto tempArray = fillArrayBaseV<TA, B>(make_index_sequence<TP::Size>{});
		size_t count = 0;
		findFirstLevelHelperArray(tempArray, t, FL);
		packArray(f, tempArray, &count);
		return tempArray;
	}
	else
	{
		auto tempArray = fillArrayBaseV<TA, B>(make_index_sequence<N>{});
		size_t count = 0;
		findFirstLevelHelperArray(tempArray, t, FL);
		packArray(f, tempArray, &count);
		return tempArray;
	}
}

template <template <class, auto> class TA, const auto & F, const auto & Tree,
	TA<TreeBase<decltype(Tree)>, getTInputTemplate<decltype(Tree)>::Size>(*DO)(decltype(F),decltype(Tree), size_t),
	size_t FL>
	struct TempWrapperCLevel {
	static constexpr auto Temp = DO(F,Tree, FL);
	static constexpr auto Res = cutSortedArray
		<F, TA, Temp.size(), TreeBase<decltype(Tree)>, Temp>();
	constexpr auto operator () () const { return Res; };
};

template <const auto & F, const auto & Tree, size_t C, template <class, auto> class TA = array>
constexpr auto findFirstLevelA()
{	return TempWrapperCLevel<TA, F, Tree, findFirstLevelArray<TA>, C>{}();}

template <const auto & F, const auto & Tree, size_t C, template <class, auto> class TA = array>
constexpr auto findLevelA()
{
	return TempWrapperCLevel<TA, F, Tree, findLevelArray<TA>, C>{}();
}

template <typename R>
constexpr size_t findLevelCount(R t, size_t FL)
{
	if constexpr (isTN<R>::istnLast())
	{
		if (FL == 0) return 1; else return 0;
	}
	else
	{
		if constexpr (!isTN <R>::istnZero())
		{
			if (FL == 0)
				return 1;					
			else
				return findLevelCount(t.left, FL-1) + findLevelCount(t.right, FL -1);
		}
		else 
			return 0;
	}
}

template <typename T, typename R>
constexpr auto findFirstLevelItem(R t, size_t L, size_t N)
{
	if constexpr (isTN<R>::istnLast())
	{
		if (L == 0)
		{
			if (N == 0)	
				return getListFistItem(t.list);
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
			if (L > 0)
			{
				if (findLevelCount(t.left, L - 1) >= (N + 1)) 
							 return  findFirstLevelItem<T>(t.left, L - 1, N);		
				else
							 return	findFirstLevelItem<T>(t.right, L - 1, N - findLevelCount(t.left, L - 1));
			}
			else if (N == 0) 
				return getListFistItem(t.list);
			else 			 
				return Zero<T>();
		} 
		else 
			return Zero<T>();
	}
}

template< template <class, auto> class TA, class T, std::size_t... I>
constexpr auto findLevelFirstHelper(T W, size_t LV, std::index_sequence<I...>)
{
	return TA<TreeBase<T>, sizeof...(I)>{findFirstLevelItem<TreeBase<T>>(W, LV, I)... };
}

template<const auto & T, size_t LV, template <class, auto> class TA = array>
constexpr auto findLevelFirst()
{
	constexpr size_t  Q = findLevelCount(T, LV);
	return   findLevelFirstHelper<TA>(T, LV, std::make_index_sequence<Q>{});
}

//max size level from all -в ширину;
template <typename R>
constexpr size_t findMaxLen(R t) {

	size_t max = 0;
	for (size_t i = 0; i < findNodeMLevel(t); i++) {
		size_t f = findLevelCount(t, i);
		if (f > max)max = f;

	}
	return max;
}
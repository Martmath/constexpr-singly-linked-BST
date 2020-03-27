#pragma once
#include <iostream>
#include <array>
#include "TreeCreatorOld.h"
#include "TreeCreator.h"
#include "CTHelper.h"

template <typename R>
constexpr auto findLevelPlaceUseHelperClass(R t, size_t L, size_t N)
{
	if constexpr (isTN<R>::istnLast())
	{
		if (L == 0)
		{
			if (N == 0)
				return Zero<size_t>();
			else
				return Zero<size_t>();
		}
		else return Zero<size_t>();
	}
	else
	{
		if constexpr (!isTN <R>::istnZero())
		{
			if (L > 0)
			{
				if (findLevelCount(t.left, L - 1) >= (N + 1))
					return  findLevelPlaceUseHelperClass(t.left, L - 1, N);
				else
					return	findLevelPlaceUseHelperClass(t.right, L - 1, N - findLevelCount(t.left, L - 1));
			}
			else if (N == 0) return t.shift;
			else			 return Zero<size_t>();
		}
		else return Zero<size_t>();
	}
}

template< template <class, auto> class TA, class T, std::size_t... I>
constexpr auto findLevelPlacesUseHelperClassHelper(T W, size_t LV, std::index_sequence<I...>)
{
	return TA<size_t, sizeof...(I)>{findLevelPlaceUseHelperClass(W, LV, I)... };
}

template<const auto & T, size_t LV, template <class, auto> class TA = array>
constexpr auto findLevelPlacesUseHelperClass()
{
	constexpr size_t  Q = findLevelCount(T, LV);
	return   findLevelPlacesUseHelperClassHelper<TA>(T, LV, std::make_index_sequence<Q>{});
}

//begin from zero c-cur level from 0,max count  S -count num
constexpr size_t firstShift(size_t C, size_t M, size_t S = 3)
{
	if ((M - C) == 1)
		return 0;
	else
	{
		if ((M - C) == 2)
			return (S >> 1) + 1;
		else
			return ((S >> 1) + 1)* ((2 << (M - C - 2)) - 1);
	}
}
//delta
constexpr size_t shiftFromPrev(size_t C, size_t M, size_t S = 3)
{
	if ((M - C) == 1)
		return (S >> 1) + 1;
	else
		return   ((S >> 1) + 1)* ((2 << (M - C - 2)));
}
//just check can or not...as all this project
template <typename R>
constexpr auto findLevelPlaceNOTUseHelperClass(R t, size_t LL, size_t N, size_t B = 0, size_t M = 0, size_t SizeVal = 3, size_t L = 0)
{
	if constexpr (isTN<R>::istnLast())
	{
		if (L == LL)
		{
			if (N == 0)
				return B;
			else
				return Zero<decltype(B)>();

		}
		else return Zero<decltype(B)>();
	}
	else
	{
		if constexpr (!isTN <R>::istnZero())
		{
			if (L < LL)
			{
				if (findLevelCount(t.left, LL - L - 1) >= (N + 1))
				{
					auto wnnhh = shiftFromPrev((L)+1, M);
					return  findLevelPlaceNOTUseHelperClass(t.left, LL, N, B - shiftFromPrev((L)+1, M, SizeVal), M, SizeVal, L + 1);
				}
				else
				{
					return	findLevelPlaceNOTUseHelperClass(t.right, LL, N - findLevelCount(t.left, LL - L - 1),
						B + shiftFromPrev((L)+1, M, SizeVal), M, SizeVal, L + 1);
				}

			}
			else if (N == 0)
				return B;
			else			 return Zero<decltype(B)>();
		}
		else return Zero<decltype(B)>();
	}
}

template< template <class, auto> class TA, class T, std::size_t... I>
constexpr auto findLevelPlacesNOTUseHelperClassHelper(T W, size_t LV, size_t SizeVal, std::index_sequence<I...>)
{
	return TA<size_t, sizeof...(I)>{findLevelPlaceNOTUseHelperClass(W, LV, I, firstShift(0, findNodeMLevel(W), SizeVal), findNodeMLevel(W), SizeVal)... };
}

template<const auto & T, size_t LV, size_t SizeVal, template <class, auto> class TA = array>
constexpr auto findLevelPlacesNOTUseHelperClass()
{
	constexpr size_t  Q = findLevelCount(T, LV);
	return   findLevelPlacesNOTUseHelperClassHelper<TA>(T, LV, SizeVal, std::make_index_sequence<Q>{});
}

template<class T>
struct WrapLevelItemaPlace {
	template<class Q> static constexpr  pair<size_t, T> Res(Q t) {
		return pair<size_t, T>{  t.shift, getListFistItem(t.list)};
	};

	static constexpr
		pair<size_t, T> Zero{ BASENUM,T{} };

};
template < typename T, template <class> class TA, typename R>//, = TreeBase<R>
constexpr auto findLevelItemaPlace(R t, size_t L, size_t N)
{
	if constexpr (isTN<R>::istnLast())
	{
		if (L == 0)
		{
			if (N == 0)	return  TA<T>::template Res<R>(t);//   t.shift;	//return decltype(t);
			else		return Zero<TA<T>>();

		}
		else return Zero<TA<T>>();
	}
	else
	{
		if constexpr (!isTN <R>::istnZero())
		{
			if (L > 0)
			{
				if (findLevelCount(t.left, L - 1) >= (N + 1))
					return  findLevelItemaPlace<T, TA>(t.left, L - 1, N);
				else
					return	findLevelItemaPlace<T, TA>(t.right, L - 1, N - findLevelCount(t.left, L - 1));
			}
			else if (N == 0)	return TA<T>::template Res<R>(t);
			else				return Zero<TA<T>>();
		}
		else return Zero<TA<T>>();
	}
}

template< template <class, auto> class TA, class T, std::size_t... I>
constexpr auto findLevelItemsaPlacesHelper(T W, size_t LV, std::index_sequence<I...>)
{
	return TA<pair<size_t, TreeBase<T>>, sizeof...(I)>{findLevelItemaPlace<TreeBase<T>, WrapLevelItemaPlace>(W, LV, I)... };
}

template<const auto & T, size_t LV, template <class, auto> class TA = array>
constexpr auto findLevelItemsaPlaces()
{
	constexpr size_t  Q = findLevelCount(T, LV);
	return   findLevelItemsaPlacesHelper<TA>(T, LV, std::make_index_sequence<Q>{});
};
//==================================
constexpr size_t a_ = firstShift(3, 5, 7);
constexpr size_t a_1 = shiftFromPrev(3, 4, 5);
constexpr int lenNum(int x) { int length = 1; while (x /= 10) length++; return length; }
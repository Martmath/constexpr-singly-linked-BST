#pragma once

#include "BaseTree.h"
#include "List.h"
#include "Array.h"


template <int N> constexpr int wwx() { return N; }
template<auto F, size_t b, class Q, class T, size_t N, template <class, auto> class TA, const TA<T, N> & Arr, int L,
	Q Min, Q Max, Q Znull, size_t d0, size_t... d/*,TLimited <Q> TT = nullptr*/> constexpr
	auto treeFromSequenceLimOld()
{
	if constexpr (sizeof...(d) == (N - b))
	{
		if constexpr ((F(Arr[d0]) > Min) && (F(Arr[d0]) < Max)
			)
			return TreeNodeLast < CListLast<T>> { CListLast<T>{Arr[d0], d0}}; //};//CListLast<T>{ Arr[d0]};
		else
			return TreeNodeLastZero{};//Zero{} sometime work,usually not
	}
	else
	{
		if constexpr ((F(Arr[d0]) > Min) && (F(Arr[d0]) < Max))
		{
			constexpr auto lb = treeFromSequenceLimOld<F, b, Q, T, N,TA, Arr, L + 1, Min, F(Arr[d0]), Znull, d...>();
			constexpr auto rb = treeFromSequenceLimOld<F, b, Q, T, N,TA, Arr, L + 1, F(Arr[d0]), Max, Znull, d...>();
			constexpr auto val = createListOld<F, T, N,TA, Arr, L + 1, N - b, d0, d...>();
			if constexpr (sizeof...(d) == (N - 1))
				return TreeNodeCount<N, L, decltype(val), decltype(lb), decltype(rb)>{val, lb, rb};
			else
				return TreeNode<L, decltype(val), decltype(lb), decltype(rb)>{val, lb, rb};
		}
		else return treeFromSequenceLimOld<F, b, Q, T, N,TA, Arr, L + 1, Min, Max, Znull, d...>();
	}
}

template<auto F, size_t a, size_t b, class Q, class T, size_t N, template <class, auto> class TA, 
	const TA<T, N> & Arr, int L, Q Min, Q Max, Q Znull, size_t d0, size_t... d> constexpr
	auto treeFromSequenceOld()
{
	if constexpr (sizeof...(d) < (N - a))
	{
		if constexpr (sizeof...(d) > (N - a - 3)) {

			if constexpr ((F(Arr[d0]) >= Min) && (F(Arr[d0]) <= Max))
			{

				constexpr auto val = createListOld<F, T, N,TA, Arr, L + 1, N - b, d0, d...>();
				return TreeNodeLast < decltype(val)> { val};
			}
			else
			{
				if constexpr (sizeof...(d) == 0) return TreeNodeLastZero{};	 else
					return treeFromSequenceOld<F, a, b, Q, T, N,TA, Arr, L + 1, Min, Max, Znull, d...>();
			}
		}
		else { return TreeNodeLastZero{}; }
	}
	else
	{
		if constexpr ((F(Arr[d0]) > Min) && (F(Arr[d0]) < Max))
		{
			constexpr auto lb = treeFromSequenceOld<F, a, b, Q, T, N,TA, Arr, L + 1, Min, F(Arr[d0]), Znull, d...>();
			constexpr auto rb = treeFromSequenceOld<F, a, b, Q, T, N,TA, Arr, L + 1, F(Arr[d0]), Max, Znull, d...>();
			auto val3 = createListOld<F, T, N,TA, Arr, L + 1, d0, d...>();
			constexpr auto val = createListOld<F, T, N,TA, Arr, L + 1, N - b, d0, d...>();
			if constexpr (sizeof...(d) == (N - 1))
				return TreeNodeCount<N, L, decltype(val), decltype(lb), decltype(rb)>{val, lb, rb};
			else
				return TreeNode<L, decltype(val), decltype(lb), decltype(rb)>{val, lb, rb};
		}
		else return treeFromSequenceOld<F, a, b, Q, T, N,TA, Arr, L + 1, Min, Max, Znull, d...>();
	}
}

template<auto F, std::size_t a, std::size_t b, class T, int N, template <class, auto> class TA, const TA<T, N> & Arr, std::size_t Imin, std::size_t Imax, std::size_t... I>
constexpr auto treeCreatorHelperOld(std::index_sequence<I...>)
{
	using outType = clearType_t<GETVINPUTTEMPLATE(F)::Tres>;
	return treeFromSequenceOld < F, a, b, outType, T, N,TA, Arr, 0,
		F(Arr[Imin]), F(Arr[Imax]), F(T{}), I... > ();
}

template<auto F, class T, int N, template <class, auto> class TA, const TA<T, N> & mA, std::size_t... I>
constexpr auto treeCreatorOld(std::index_sequence<I...>)
{
	constexpr	auto  A = cxArray::prepareBalancedIndexesOld<F>(mA);
	constexpr	auto L = get<0>(A);
	return  treeCreatorHelperOld<F, get<1>(A), get<2>(A), T, N,TA, mA,	L[get<1>(A)], L[get<1>(A) + 1]
	>(index_sequence <L[I]...>());
}

template<auto F, class T, int N, template <class, auto> class TA, const TA<T, N> & mA>
constexpr auto treeCreatorOld()
{
	return treeCreatorOld<F, T, N,TA, mA>(std::make_index_sequence<N>{});
}

template<auto F, std::size_t b, class T, int N, template <class, auto> class TA, const TA<T, N> & Arr, std::size_t Imin, std::size_t Imax, std::size_t... I>
constexpr auto treeCreatorLimHelperOld(std::index_sequence<I...>)
{
	using outType = clearType_t<GETVINPUTTEMPLATE(F)::Tres>;
	return treeFromSequenceLimOld < F, b, outType, T, N,TA, Arr, 0,
		numeric_limits<outType>::min(), numeric_limits<outType>::max(), F(T{}), I... > ();

}

template<auto F, class T, int N, template <class, auto> class TA, const TA<T, N> & mA, std::size_t... I>
constexpr auto treeCreatorLimOld(std::index_sequence<I...>)
{
	constexpr	auto A = cxArray::prepareBalancedIndexesNumLimOld<F>(mA);	
	constexpr	auto L = get<0>(A);
	return  treeCreatorLimHelperOld<F, get<2>(A), T, N,TA, mA, L[N - 2], L[N - 1]>(index_sequence <L[I]...>());
}

template<auto F, class T, int N, template <class, auto> class TA, const TA<T, N> & mA>
constexpr auto treeCreatorLimOld()
{
	return treeCreatorLimOld<F, T, N,TA, mA>(std::make_index_sequence<N>{});
}


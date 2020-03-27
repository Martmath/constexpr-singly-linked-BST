#pragma once

#include "BaseTree.h"
#include "List.h"
#include "Array.h"


template<auto F, size_t N1, class T, size_t N, template <class, auto> class TA,const TA<T, N> & Arr, int L,
	const auto Min, const auto Max, size_t d0, size_t... d> constexpr
	auto treeFromSequenceNumLim() {
	if constexpr (sizeof...(d) == 0)
	{
		if constexpr ((F(Arr[d0]) > Min) && (F(Arr[d0]) < Max)) 
		{
			constexpr auto val = createList<F, T, N,TA, Arr, L + 1, d0, d...>();
			return TreeNodeLast < decltype(val)> { val};
		}
		else
			return TreeNodeLastZero{};
	}
	else
	{
		if constexpr ((F(Arr[d0]) > Min) && (F(Arr[d0]) < Max))
		{
			constexpr auto lb = treeFromSequenceNumLim<F, N1, T, N,TA, Arr, L + 1, Min, F(Arr[d0]), d...>();
			constexpr auto rb = treeFromSequenceNumLim<F, N1, T, N,TA, Arr, L + 1, F(Arr[d0]), Max, d...>();
			constexpr auto val = createList<F, T, N,TA, Arr, L + 1, d0, d...>();
			if constexpr (sizeof...(d) == (N1 - 1))
				return TreeNodeCount<sizeof...(d) + 1, L, decltype(val), decltype(lb), decltype(rb)>{val, lb, rb};
			else
				return TreeNode<L, decltype(val), decltype(lb), decltype(rb)>{val, lb, rb};
		}
		else return treeFromSequenceNumLim<F, N1, T, N,TA, Arr, L + 1, Min, Max, d...>();
	}
}

template<auto F, class T, size_t N, template <class, auto> class TA, const TA<T, N> & Arr, size_t... d> constexpr
auto treeFromSequenceNumLim(std::index_sequence<d...>) {

	using outType = clearType_t<GETVINPUTTEMPLATE(F)::Tres>;

	return treeFromSequenceNumLim< F, sizeof...(d), T, N,TA, Arr, 0,
		numeric_limits<outType>::min(), numeric_limits<outType>::max(), d... >();
}



template<auto F, class T, size_t N, template <class, auto> class TA, const TA<T, N> & mA, size_t L,
	size_t MinI, size_t MaxI, size_t MinC, size_t MaxC, std::size_t I0, std::size_t... I>
	constexpr auto treeFromSequenceTNC()
{
	constexpr auto lb = treeFromSequence<F, T, N,TA, mA, L, MinI, I0, MinC, MaxC, I...>();
	constexpr auto rb = treeFromSequence < F, T, N,TA, mA, L, I0, MaxI, MinC, MaxC, I...>();
	constexpr auto val = createList<F, T, N,TA, mA, L + 1, I0, I...>();
	return TreeNodeCount<sizeof...(I), L, decltype(val), decltype(lb), decltype(rb)>{val, lb, rb};
}




template<auto F, class T, size_t N, template <class, auto> class TA, const TA<T, N> & mA, size_t MinI, size_t MaxI, size_t MinC, size_t MaxC, std::size_t... I>
constexpr auto treeFromSequenceTNC(std::index_sequence<I...>)
{
	return treeFromSequenceTNC<F, T, N,TA, mA, 0, MinI, MaxI, MinC, MaxC, I...>();
}

template<auto F, class T, int N, template <class, auto> class TA, const TA<T, N> & mA,
	size_t L, size_t MinI, size_t MaxI, size_t MinC, size_t MaxC, size_t I0, size_t... I>
	constexpr auto treeFromSequence()
{
	if constexpr ((sizeof...(I) == MinC) || (sizeof...(I) == MaxC))
	{
		if constexpr ((F(mA[I0]) >= F(mA[MinI])) && (F(mA[I0]) <= F(mA[MaxI])))
		{
			constexpr auto val = createList<F, T, N,TA, mA, L + 1, I0, I...>();
			return TreeNodeLast < decltype(val)> { val};
		}
		else
		{
			if constexpr (sizeof...(I) == MaxC)
				return TreeNodeLastZero{};
			else
				return  treeFromSequence < F, T, N,TA, mA, L, MinI, MaxI, MinC, MaxC, I...>();
		}
	}
	else
	{
		if constexpr ((F(mA[I0]) > F(mA[MinI])) && (F(mA[I0]) < F(mA[MaxI])))
		{
			constexpr auto lb = treeFromSequence<F, T, N, TA,mA, L + 1, MinI, I0, MinC, MaxC, I...>();
			constexpr auto rb = treeFromSequence < F, T, N,TA, mA, L + 1, I0, MaxI, MinC, MaxC, I...>();
			constexpr auto val = createList<F, T, N,TA, mA, L + 1, I0, I...>();
			return TreeNode<L, decltype(val), decltype(lb), decltype(rb)>{val, lb, rb};
		}
		else
			return	 treeFromSequence < F, T, N,TA, mA, L, MinI, MaxI, MinC, MaxC, I...>();
	}
}

template<auto F, class T, size_t N, template <class, auto> class TA, const TA<T, N> & mA, std::size_t... I>
constexpr auto treeCreatorLim(std::index_sequence<I...>)
{
	constexpr	auto A = cxArray::prepareBalancedIndexesLim<F>(mA);
	constexpr	auto L = get<TPLRes::Array>(A);
	return	treeFromSequenceNumLim<F, T, N,TA, mA>(cxArray::cutSec<get<TPLRes::PlaceNotZero>(A)>
		(index_sequence<L[I]...>{}));
}

template<auto F, class T, size_t N, template <class, auto> class TA, const TA<T, N> & mA>
constexpr auto treeCreatorLim()
{
	return treeCreatorLim<F, T, N, TA, mA>(std::make_index_sequence<N>{});
}

template<auto F, class T, size_t N, template <class, auto> class TA, const TA<T, N> & mA, std::size_t... I>
constexpr auto treeCreator(std::index_sequence<I...>)
{
	constexpr	auto  A = cxArray::prepareBalancedIndexes<F>(mA);
	constexpr	auto L = get<TPLRes::Array>(A);
	return treeFromSequenceTNC
		<F, T, N, TA, mA, L[get<TPLRes::PlaceMin>(A)], L[get<TPLRes::PlaceMax>(A)],
		get<TPLRes::PlaceNotZero>(A) - get<TPLRes::PlaceMin>(A) - 1, get<TPLRes::PlaceNotZero>(A) - get<TPLRes::PlaceMax>(A) - 1>
		(cxArray::cutSec<get<TPLRes::PlaceNotZero>(A)>(index_sequence<L[I]...>{}));

}

template<auto F, class T, size_t N, template <class, auto> class TA, const TA<T, N> & mA>
constexpr auto treeCreator()
{
	return treeCreator<F, T, N,TA, mA>(std::make_index_sequence<N>{});
}


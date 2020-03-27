#pragma once
#include "BaseTree.h"
#include "List.h"
#include "Array.h"

template<auto F, class Q, class T, size_t N, template <class, auto> class TA, const TA<T, N> & Arr, int L,
	Q Min, Q Max, size_t d0, size_t... d/*,TLimited <Q> TT = nullptr*/> constexpr
	auto treeNodeFromSequence() {
	if constexpr (sizeof...(d) == 0)
	{
		if constexpr ((F(Arr[d0]) > Min) && (F(Arr[d0]) < Max))
			return TreeNodeLast < CListLast<T>> { CListLast<T>{Arr[d0], d0}};
		else
			return TreeNodeLastZero{};//Zero{} not work
	}
	else
	{
		if constexpr ((F(Arr[d0]) > Min) && (F(Arr[d0]) < Max))
		{
			constexpr auto lb = treeNodeFromSequence<F, Q, T, N,TA, Arr, L + 1, Min, F(Arr[d0]), d...>();
			constexpr auto rb = treeNodeFromSequence<F, Q, T, N, TA, Arr, L + 1, F(Arr[d0]), Max, d...>();
			constexpr auto val = createList<F, T, N, TA, Arr, L + 1, d0, d...>();
			///CListLast<T>{ Arr[d0], d0 };//getSameVs<F, T, N, Arr, L + 1, d0, d0, d...>();
			if constexpr (sizeof...(d) == (N - 1))
				return TreeNodeCount<N, L, decltype(val), decltype(lb), decltype(rb)>{val, lb, rb};
			else
				return TreeNode<L, decltype(val), decltype(lb), decltype(rb)>{val, lb, rb};
		}
		else return treeNodeFromSequence<F, Q, T, N,TA, Arr, L + 1, Min, Max, d...>();
	}
}

template<auto F, const auto & mA, const auto min, const auto max, std::size_t... I>
constexpr auto treeCreatorNodeHelper(std::index_sequence<I...>)
{
	using T = getTInputTemplate<decltype (mA)>;
	return treeNodeFromSequence<F, decltype(min), typename T::Tin, T::Size, T::template TAR, mA, 0, min, max, I...>();
}

template<auto F, const auto & mA>
constexpr auto treeCreatorNode()
{
	using T = getTInputTemplate<decltype (mA)>;
	using TT = getTInputTemplate<decltype (F)>;
	static_assert(NL<TT::Ctres>, "try calling treeCreatorNode function with a minimum and maximum");
	return treeCreatorNodeHelper<F, mA, numeric_limits<TT::Ctres>::min(), numeric_limits<TT::Ctres>::max()>(std::make_index_sequence<T::Size>{});
}

template<auto F, const auto & mA, const auto Min, const auto Max>
//class TT = getTInputTemplate<decltype (F)>>, class Ttres = TT::Ctres
constexpr auto treeCreatorNode()
{
	using T = getTInputTemplate<decltype (mA)>;
	using TT = getTInputTemplate<decltype (F)>;
	static_assert(std::is_same_v<TT::Ctres, clearType_t<decltype (Min)>> &&
		std::is_same_v<TT::Ctres, clearType_t<decltype (Max)>> &&
		std::is_same_v<TT::Ctin, clearType_t<T::Tin>>, "type isnt same");
	static_assert(itOkForTree<TT::Ctres>{}, "bad type of item array");
	return treeCreatorNodeHelper<F, mA, Min, Max>(std::make_index_sequence<T::Size>{});
}

template<auto F, const auto & mA, const auto & Min, const auto & Max, class TUnpackArray = getTInputTemplate<clearType_t<decltype(mA)>>>
constexpr auto treeCreatorNode()
{
	using T = getTInputTemplate<decltype (mA)>;
	using TT = getTInputTemplate<decltype (F)>;
	static_assert(std::is_same_v<TT::Ctres, clearType_t<decltype (Min)>> &&
		std::is_same_v<TT::Ctres, clearType_t<decltype (Max)>> &&
		std::is_same_v<TT::Ctin, clearType_t<T::Tin>>, "input types isnt same");
	static_assert(itOkForTree<TT::Ctres>{}, "bad type of item array");
	return treeCreatorNodeHelper<F, mA, Min, Max>(std::make_index_sequence<T::Size>{});
}



/*template<auto F, class T, size_t N, const array<T, N> & Arr,
	const int L, const size_t Me, std::size_t... I>
	constexpr auto getSameVsHelper(std::index_sequence<I...>)
{	return getSameVs<F, T, N, Arr, L + 1, Me, I...>();}*/


/*template<auto F, class T, int N, const array<T, N> & Arr,
	const int L, const int Me, const int d0, const int... d>
	constexpr auto getSameVs() {
	if constexpr (sizeof...(d) == 0)
	{
		if constexpr (F(Arr[d0]) == F(Arr[Me]))
			return CListLast<T>{ Arr[d0], d0};
		else
			return CListLastZero{};
	}
	else
	{
		if constexpr (F(Arr[d0]) == F(Arr[Me]))
		{
			constexpr auto tail_ = getSameVs<F, T, N, Arr, L + 1, d0, d...>();
			return CList<L, T, decltype(tail_)> {Arr[d0], tail_, d0 };
		}
		else
		{
			return getSameVs<F, T, N, Arr, L + 1, Me, d...>();
		}
	}
}*/
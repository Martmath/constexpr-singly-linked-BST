#pragma once
#include "BaseTree.h"

template<size_t X, size_t Level, class C1, class C2, class C3 >
constexpr auto CreateNodePlaceHelper(const C1& D, const C2& D2, const C3& D3, size_t Size, size_t Shift = 9)
{
	return TreeNodeFull<X, C1, C2, C3, Level>{D, D2, D3, Size, Shift};
}

template<size_t X, size_t SIZE, size_t Level, class C1, class C2, class C3 >
constexpr auto CreateNodePlaceHelper(const C1& D, const C2& D2, const C3& D3, size_t Size, size_t Shift = 6)
{
	return TreeNodeFullCount<SIZE, X, C1, C2, C3, Level>{D, D2, D3, Size, Shift};
}

template<size_t Level = 0, class C >
constexpr auto CreateTreePlaceHelper(const C& D, size_t CurrentPlace = 4, size_t MaxSize = 4, size_t Len = 3)
{
	using T = getTInputTemplate<clearType_t<C>>;

	if constexpr (isTN <C>::istnZero())	return TreeNodeFullLastZero{};
	else {
		if constexpr (isTN <C>::istnLast())
			return TreeNodeFullLast<T::D, Level>{ D.list, listCount(D.list), CurrentPlace };
		else
		{
			using TL = getTInputTemplate<clearType_t<decltype(D.left)>>;
			using TR = getTInputTemplate<clearType_t<decltype(D.right)>>;
			if constexpr (isTN<C>::it() == tnSimple)
				return CreateNodePlaceHelper<T::X, Level>(D.list,
					CreateTreePlaceHelper<Level + 1>(D.left, CurrentPlace - shiftFromPrev(Level + 1, MaxSize,Len), MaxSize,Len),
					CreateTreePlaceHelper<Level + 1>(D.right, CurrentPlace + shiftFromPrev(Level + 1, MaxSize,Len), MaxSize,Len),
					findNodeCount(D), CurrentPlace);
			else
				return CreateNodePlaceHelper<T::X, T::Size, Level>(D.list,
					CreateTreePlaceHelper<Level + 1>(D.left, CurrentPlace - shiftFromPrev(Level + 1, MaxSize, Len), MaxSize,Len),
					CreateTreePlaceHelper<Level + 1>(D.right, CurrentPlace + shiftFromPrev(Level + 1, MaxSize, Len), MaxSize,Len),
					findNodeCount(D), CurrentPlace);
		}
	}
};
template<size_t Level = 0, class C >
constexpr auto CreateTreePlace(const C& D,  size_t Len = 3)
{
	if constexpr (sameZ<C>())
	{
		return CZero{};
	}
	else 
	{
		return CreateTreePlaceHelper(D, firstShift(0, findNodeMLevel(D), Len), findNodeMLevel(D), Len);
	}
}


//constexpr auto qvj = modifyNodeOffset<findNodeCount(TQ.right), 3>(TQ.right);
template<size_t SIZE = 0, size_t Delta= 0, class C>
constexpr auto modifyNodeOffset(const C& D)
{
	using T = getTInputTemplate<clearType_t<C>>;

	if constexpr (isTN <C>::istnZero())	return TreeNodeLastZero{};
	else {
		if constexpr (isTN <C>::istnLast())
			return TreeNodeLast<T::D>{ D.list };
		else
		{	
			if constexpr (SIZE == 0)
			{
				if constexpr (isTN<C>::it() == tnSimple)
					return

					TreeNode<T::X + Delta, decltype(D.list), decltype(modifyNodeOffset(D.left)), decltype(modifyNodeOffset(D.right)) >{D.list, modifyNodeOffset(D.left), modifyNodeOffset(D.right)};
				else
					return
					TreeNodeCount<T::Size, T::X + Delta, decltype(D.list), decltype(modifyNodeOffset(D.left)), decltype(modifyNodeOffset(D.right))>{D.list, modifyNodeOffset(D.left), modifyNodeOffset(D.right)};//findNodeCount(D)
			}
			else
				return
				TreeNodeCount<SIZE, T::X+Delta, decltype(D.list), decltype(modifyNodeOffset(D.left)), decltype(modifyNodeOffset(D.right))>{D.list, modifyNodeOffset(D.left), modifyNodeOffset(D.right)};//findNodeCount(D)
		}
	}
};

/* :((
template<auto F, FuncRes<decltype(F)> c,typename T = FuncIn<decltype(F)>, typename R>
constexpr auto findF(R t)
{
	if constexpr (isTN<R>::istnZero()) 	return TreeNodeLastZero{};
	else
	{
		if constexpr (isTN<R>::istnLast())
		{
			if constexpr (F(t.list.head) == c) return modifyNodeOffset(t);
			else return TreeNodeLastZero{};
		}
		else
		{
			if constexpr (F(t.list.head) == c) 
				return modifyNodeOffset(t);
			else
			{
				if  constexpr (F(t.list.head) < c) 
					return findF<F,c>(t.right);
				else 
					return findF<F,c>(t.left);
			}
		}
	}
}
*/


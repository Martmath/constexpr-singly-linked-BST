#pragma once
#include "OutSupport.h"
template<size_t X, class D, class L, class R> struct TreeNode
{
	const D list;
	const L left{};
	const R right{};
};

template<size_t X, class D> struct TreeNode<X, D, CZero, CZero>
{
	const D list;
};

template<> struct TreeNode<0, CZero, CZero, CZero> {};
template<size_t X, class D> using TreeNodeLastHelper = TreeNode<X, D, CZero, CZero>;
template<class D> using TreeNodeLast = TreeNodeLastHelper<1, D>;
using TreeNodeLastZero = TreeNodeLastHelper<0, CZero>;

template<class T>
using TreeBase = typename  getTInputTemplate <typename getTInputTemplate<T>::D>::H;
//template<class T>
//using TreeBaseV = typename  getTInputTemplate <typename getTInputTemplate<T>::D>::H;

template<class T> using FuncRes = typename getTInputTemplate<T>::Tres;
template<class T> using FuncIn = typename getTInputTemplate<T>::Tin;

//template<const auto& F> using FuncResV = typename getTInputTemplate<decltype(F)>::Tres; -not work!!
//template<const auto & F> using FuncInV = typename getTInputTemplate<decltype(F)>::Tin;
template<size_t SIZE, size_t X, class  D, class L, class R>
struct TreeNodeCount :public TreeNode<X, D, L, R>
{
	static constexpr size_t Size = SIZE;
};

template<size_t X, class  D, class L, class R, size_t LEVEL = 0>
struct TreeNodeFull :public TreeNode<X, D, L, R>
{
	const size_t size=0;
	const size_t shift=0;
};

template<size_t SIZE, size_t X, class  D, class L, class R,size_t LEVEL = 0>
struct TreeNodeFullCount :public TreeNodeCount<SIZE, X, D, L, R>
{
	const size_t size=0;
	const size_t shift = 0;
};

template<size_t X, class D, size_t LEVEL> 
struct TreeNodeFull <X, D, CZero, CZero, LEVEL>
{
	const D list;
	const size_t size=0;
	const size_t shift = 0;
};

template<> struct TreeNodeFull<0, CZero, CZero, CZero, 0> {};
template<size_t X, class D, size_t LEVEL> 
using TreeNodeFullLastHelper = TreeNodeFull<X, D, CZero, CZero, LEVEL>;
template<class D, size_t LEVEL> using TreeNodeFullLast =
TreeNodeFullLastHelper<1, D, LEVEL>;
using TreeNodeFullLastZero = TreeNodeFullLastHelper<0, CZero,0>;







//https://stackoverflow.com/questions/31762958/check-if-class-is-a-template-specialization
enum  TreeNodeType { tnZero = 0, tnLast, tnSimple, tnOther };
template <class T>
struct isTN {
	constexpr static TreeNodeType it() { return tnOther; };
	constexpr static bool istnZero() { return false; };
	constexpr static bool istnLast() { return false; };
};
template <class T>constexpr bool sameZero() { return std::is_same<T, CZero>(); };
template <class InClass, class Left, class Right, size_t I>
struct isTN<TreeNode<I, InClass, Left, Right>>
{
	using CL = InClass;
	constexpr static TreeNodeType it()
	{
		if (sameZero<Left>() && sameZero<Right>()) return  (I == 0) ? tnZero : tnLast;
		else return tnSimple;
	}
	constexpr static bool istnZero() { return sameZero<Left>() && sameZero<Right>() && (I == 0); };
	constexpr static bool istnLast() { return sameZero<Left>() && sameZero<Right>() && (I != 0); };
};


/*template <class T>
struct isTN {
	constexpr static TreeNodeType it() { return tnOther; };
	constexpr static bool istnZero() { return false; };
	constexpr static bool istnLast() { return false; };
};*/
//template <class T>constexpr bool sameZero() { return std::is_same<T, CZero>(); };
template <class InClass, class Left, class Right, size_t I, size_t I1>
struct isTN<TreeNodeFull<I, InClass, Left, Right,I1>>
{
	using CL = InClass;
	constexpr static TreeNodeType it()
	{
		if (sameZero<Left>() && sameZero<Right>()) return  (I == 0) ? tnZero : tnLast;
		else return tnSimple;
	}
	constexpr static bool istnZero() { return sameZero<Left>() && sameZero<Right>() && (I == 0); };
	constexpr static bool istnLast() { return sameZero<Left>() && sameZero<Right>() && (I != 0); };
};






template<auto F, typename T, typename Res>constexpr bool contains(T t, const Res c)
{
	if constexpr (isTN <T>::it() == tnZero)
		return false;//is_same<T, D0>()
	else
	{
		if constexpr (isTN <T>::it() == tnLast)
		{
			if (F(t.list.head) == c) return true; else return false;
		}
		else
		{
			if (F(t.list.head) == c) return true;
			else if (F(t.list.head) < c) return contains<F>(t.right, c);
			else return contains<F>(t.left, c);
		}
	}
};

template<auto F, typename T>
constexpr GETVINPUTTEMPLATE_IN(F) findFirst(T t, const GETVINPUTTEMPLATE_RES(F) c)
{
	if constexpr (isTN <T>::it() == tnZero)  return  GETVINPUTTEMPLATE_IN(F) {};
	else
	{
		if constexpr (isTN <T>::it() == tnLast)
		{
			if (F(t.list.head) == c) return t.list.head; else return GETVINPUTTEMPLATE_IN(F) {};
		}
		else
		{
			if (F(t.list.head) == c) return t.list.head;
			else if (F(t.list.head) < c) return findFirst<F>(t.right, c);
			else return findFirst<F>(t.left, c);
		}
	}
}

template <  typename R>
constexpr size_t findNodeCount(R t)
{
	if constexpr (isTN <R>::it() == tnLast)
	{
		return  listCount(t.list);
	}
	else
	{
		if constexpr (isTN <R>::it() != tnZero)
		{
			return listCount(t.list) + findNodeCount(t.left) + findNodeCount(t.right);
		}
		else {
			return  0;
		}
	}
}

template <typename R>
constexpr size_t findNodeMLevel(R t)
{
	if constexpr (isTN <R>::it() == tnLast)
	{
		return  1;
	}
	else
	{
		if constexpr (!(sameZ<R>()||(isTN <R>::it() == tnZero)))
		{
			if (findNodeMLevel(t.left) > findNodeMLevel(t.right))
				return findNodeMLevel(t.left) + 1;
			else
				return findNodeMLevel(t.right) + 1;
		}
		else {
			return  0;
		}
	}
}
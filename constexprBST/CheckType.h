#pragma once
#include "BaseSupport.h"
#pragma region CheckTypeForTree

template<class T, class EqualTo> struct hasOperatorsForTree {
	template<class U, class V>
	static decltype(
		(std::declval<U>() == std::declval<V>()) &&
		//(std::declval<U>() != std::declval<V>()) &&
		(std::declval<U>() > std::declval<V>()) &&
		(std::declval<U>() < std::declval<V>()) &&
		(std::declval<U>() >= std::declval<V>()) &&
		(std::declval<U>() <= std::declval<V>())
		) test(U*) {};
	template<typename, typename> static std::false_type test(...) {};

#ifdef SpaceShipOperator
	template<class U, class V>
	static decltype((std::declval<U>() <=> std::declval<V>())) test1(U*) {};
#endif
	template<typename, typename> static std::false_type test1(...) {};

	using type = typename bool_constant<(!is_same_v<std::false_type, decltype(test<T, EqualTo>(0))>) ||
		(!is_same_v<std::false_type, decltype(test1<T, EqualTo>(0))>) >::type;
};

template <class T>constexpr bool NL = (numeric_limits<T>::is_specialized);
template<class T, class EqualTo = T> struct itOkForTree : hasOperatorsForTree<T, EqualTo>::type {};




template<class T> struct hasZeroT {
	template<class U>
	static decltype	(U::Zero) test(U*) {};
	template<typename> static std::false_type test(...) {};

	using type = typename bool_constant<(!is_same_v<std::false_type, decltype(test<T>(0))>)>::type;
};
template<class T> struct hasZeroResT : hasZeroT<T>::type {};


template<class T> struct hasSizeT {
	template<class U>
	static decltype	(U::Size) test(U*) {};
	template<typename> static std::false_type test(...) {};

	using type = typename bool_constant<(!is_same_v<std::false_type, decltype(test<T>(0))>)>::type;
};




template<class T> struct hasSizeResT : hasSizeT<T>::type {};

template<const auto & T> struct hasSizeV {
	template<const auto & U>
	static decltype(U.size()) test(decltype(U.size())*) {};
	template<const auto&> static std::false_type test(...) {};

	using type = typename bool_constant<(!is_same_v<std::false_type, decltype(test<T>(0))>)>::type;
};
template<const auto & T> struct hasSizeResV : hasSizeV<T>::type {};


template<const auto & T> constexpr
static auto getsizeV() { return T.size(); }

template<class T> constexpr
static auto getSizeT() { return T::Size; }

template<const auto  T> constexpr
static auto getSize()
{
	return 0;
}

template<const auto & T> constexpr
static auto getSize()
{
	if constexpr (hasSizeResV<T>{})
	{
		return getsizeV<T>();
	}
	else
	{
		if constexpr (hasSizeResT<getTInputTemplate<decltype(T)>>{})
		{
			return  getSizeT<getTInputTemplate<decltype(T)>>();
		}
		else	return 0;
	}
}

#pragma endregion



#pragma region getInputTemplate

#pragma region clearType
template <class Tin, bool ClearPointer = false> struct clearType { using T = Tin; };

template <class Tin> struct clearType<const Tin, false> { using T = Tin; };
template <class Tin> struct clearType<Tin&, false> { using T = Tin; };
template <class Tin> struct clearType<const Tin&, false> { using T = Tin; };

template <class Tin> struct clearType<const Tin, true> { using T = Tin; };
template <class Tin> struct clearType<Tin&, true> { using T = Tin; };
template <class Tin> struct clearType<const Tin&, true> { using T = Tin; };
template <class Tin> struct clearType<Tin*, true> { using T = Tin; };
template <class Tin> struct clearType<const Tin*, true> { using T = Tin; };
template <class Tin, bool ClearPointer = false> using clearType_t
= typename clearType<Tin, ClearPointer>::T;
#pragma endregion

#pragma region getInputTemplate

enum  typeVar { Func = 0, Arr, Tree, Other };

template <class T>struct getTInputTemplate
{
	static constexpr typeVar Type = typeVar::Other;
	//static_assert(true, "error found Pattern");
};


//static 



template <class TRes, class TIn>struct getTInputTemplate<TRes(TIn)>
{
	static constexpr typeVar Type = typeVar::Func;
	using Tres = TRes; using Tin = TIn;
	using Ctres = clearType_t<TRes>; using Ctin = clearType_t<TIn>;
};

template <class TRes, class TIn>struct getTInputTemplate<TRes(__cdecl*)(TIn)>
{
	static constexpr typeVar Type = typeVar::Func;
	using Tres = TRes; using Tin = TIn;
	using Ctres = clearType_t<TRes>; using Ctin = clearType_t<TIn>;
};

//==========================================================================
template <auto _X, class _H, class _T, template <auto, class, class> class _TT>
struct getTInputTemplate<_TT<_X, _H, _T>>
{
	static constexpr typeVar Type = typeVar::Tree;
	static constexpr decltype(_X)  X = _X;
	using H = _H;
	using T = _T;
	template <auto X_, class H_, class T_> using TAR = _TT<X_, H_, T_>;
};

template <auto _X, class _D, class _L, class _R, template <auto, class, class, class> class _TT>
struct getTInputTemplate<_TT<_X, _D, _L, _R>>
{
	static constexpr typeVar Type = typeVar::Tree;
	static constexpr decltype(_X)  X = _X;
	using D = _D;
	using L = _L;
	using R = _R;
	template <auto X_, class D_, class L_, class R_> using TAR = _TT<X_, D_, L_, R_>;
};

template <auto _X, class _D, class _L, class _R, auto _LEVEL,
	template <auto, class, class, class, auto> class _TT>
struct getTInputTemplate<_TT<_X, _D, _L, _R, _LEVEL>>
{
	static constexpr typeVar Type = typeVar::Tree;
	static constexpr decltype(_X)  X = _X;
	using D = _D;
	using L = _L;
	using R = _R;
	static constexpr decltype(_LEVEL)  LEVEL = _LEVEL;
	template <auto X_, class D_, class L_, class R_, auto LEVEL_> using TAR =
		_TT<X_, D_, L_, R_, LEVEL_>;
};

template <auto _SIZE, auto _X, class _D, class _L, class _R, auto _LEVEL,
	template <auto,auto, class, class, class, auto> class _TT>
struct getTInputTemplate<_TT<_SIZE, _X, _D, _L, _R, _LEVEL>>
{
	static constexpr typeVar Type = typeVar::Tree;
	static constexpr decltype(_SIZE)  SIZE = _SIZE;
	static constexpr decltype(_X)  X = _X;
	using D = _D;
	using L = _L;
	using R = _R;	
	static constexpr decltype(_LEVEL)  LEVEL = _LEVEL;
	template <auto SIZE_, auto X_, class D_, class L_, class R_, auto LEVEL_> using TAR =
		_TT<SIZE_, X_, D_, L_, R_, LEVEL_>;
};



template <auto SIZE, auto _X, class _D, class _L, class _R, template <auto, auto, class, class, class> class _TT>
struct getTInputTemplate<_TT<SIZE, _X, _D, _L, _R>>
{
	static constexpr typeVar Type = typeVar::Tree;
	static constexpr decltype(SIZE)  Size = SIZE;
	static constexpr decltype(_X)  X = _X;
	using D = _D;
	using L = _L;
	using R = _R;	
	template <auto SIZE_, auto X_, class D_, class L_, class R_> using TAR = _TT<SIZE_, X_, D_, L_, R_>;
};

//========================================
template <auto _X, class _H, class _T, template <auto, class, class> class _TT>
struct getTInputTemplate<const _TT<_X, _H, _T>>
{
	static constexpr typeVar Type = typeVar::Tree;
	static constexpr decltype(_X)  X = _X;
	using H = _H;
	using T = _T;
	template <auto X_, class H_, class T_> using TAR = _TT<X_, H_, T_>;
};

template <auto _X, class _D, class _L, class _R, template <auto, class, class, class> class _TT>
struct getTInputTemplate<const _TT<_X, _D, _L, _R>>
{
	static constexpr typeVar Type = typeVar::Tree;
	static constexpr decltype(_X)  X = _X;
	using D = _D;
	using L = _L;
	using R = _R;
	template <auto X_, class D_, class L_, class R_> using TAR = _TT<X_, D_, L_, R_>;
};

template <auto _X, class _D, class _L, class _R, auto _LEVEL,
	template <auto, class, class, class, auto> class _TT>
struct getTInputTemplate<const _TT<_X, _D, _L, _R, _LEVEL>>
{
	static constexpr typeVar Type = typeVar::Tree;
	static constexpr decltype(_X)  X = _X;
	using D = _D;
	using L = _L;
	using R = _R;
	static constexpr decltype(_LEVEL)  LEVEL = _LEVEL;
	template <auto X_, class D_, class L_, class R_, auto LEVEL_> using TAR =
		_TT<X_, D_, L_, R_, LEVEL_>;
};

template <auto _SIZE, auto _X, class _D, class _L, class _R, auto _LEVEL,
	template <auto, auto, class, class, class, auto> class _TT>
struct getTInputTemplate<const _TT<_SIZE, _X, _D, _L, _R, _LEVEL>>
{
	static constexpr typeVar Type = typeVar::Tree;
	static constexpr decltype(_SIZE)  SIZE = _SIZE;
	static constexpr decltype(_X)  X = _X;
	using D = _D;
	using L = _L;
	using R = _R;
	static constexpr decltype(_LEVEL)  LEVEL = _LEVEL;
	template <auto SIZE_, auto X_, class D_, class L_, class R_, auto LEVEL_> using TAR =
		_TT<SIZE_, X_, D_, L_, R_, LEVEL_>;
};



template <auto SIZE, auto _X, class _D, class _L, class _R, template <auto, auto, class, class, class> class _TT>
struct getTInputTemplate<const _TT<SIZE, _X, _D, _L, _R>>
{
	static constexpr typeVar Type = typeVar::Tree;
	static constexpr decltype(SIZE)  Size = SIZE;
	static constexpr decltype(_X)  X = _X;
	using D = _D;
	using L = _L;
	using R = _R;
	template <auto SIZE_, auto X_, class D_, class L_, class R_> using TAR = _TT<SIZE_, X_, D_, L_, R_>;
};
//================

template <auto _X, class _H, class _T, template <auto, class, class> class _TT>
struct getTInputTemplate<const _TT<_X, _H, _T>&>
{
	static constexpr typeVar Type = typeVar::Tree;
	static constexpr decltype(_X)  X = _X;
	using H = _H;
	using T = _T;
	template <auto X_, class H_, class T_> using TAR = _TT<X_, H_, T_>;
};

template <auto _X, class _D, class _L, class _R, template <auto, class, class, class> class _TT>
struct getTInputTemplate<const _TT<_X, _D, _L, _R>&>
{
	static constexpr typeVar Type = typeVar::Tree;
	static constexpr decltype(_X)  X = _X;
	using D = _D;
	using L = _L;
	using R = _R;
	template <auto X_, class D_, class L_, class R_> using TAR = _TT<X_, D_, L_, R_>;
};

template <auto _X, class _D, class _L, class _R, auto _LEVEL,
	template <auto, class, class, class, auto> class _TT>
struct getTInputTemplate<const _TT<_X, _D, _L, _R, _LEVEL>&>
{
	static constexpr typeVar Type = typeVar::Tree;
	static constexpr decltype(_X)  X = _X;
	using D = _D;
	using L = _L;
	using R = _R;
	static constexpr decltype(_LEVEL)  LEVEL = _LEVEL;
	template <auto X_, class D_, class L_, class R_, auto LEVEL_> using TAR =
		_TT<X_, D_, L_, R_, LEVEL_>;
};

template <auto _SIZE, auto _X, class _D, class _L, class _R, auto _LEVEL,
	template <auto, auto, class, class, class, auto> class _TT>
struct getTInputTemplate<const _TT<_SIZE, _X, _D, _L, _R, _LEVEL>&>
{
	static constexpr typeVar Type = typeVar::Tree;
	static constexpr decltype(_SIZE)  SIZE = _SIZE;
	static constexpr decltype(_X)  X = _X;
	using D = _D;
	using L = _L;
	using R = _R;
	static constexpr decltype(_LEVEL)  LEVEL = _LEVEL;
	template <auto SIZE_, auto X_, class D_, class L_, class R_, auto LEVEL_> using TAR =
		_TT<SIZE_, X_, D_, L_, R_, LEVEL_>;
};



template <auto SIZE, auto _X, class _D, class _L, class _R, template <auto, auto, class, class, class> class _TT>
struct getTInputTemplate<const _TT<SIZE, _X, _D, _L, _R>&>
{
	static constexpr typeVar Type = typeVar::Tree;
	static constexpr decltype(SIZE)  Size = SIZE;
	static constexpr decltype(_X)  X = _X;
	using D = _D;
	using L = _L;
	using R = _R;
	template <auto SIZE_, auto X_, class D_, class L_, class R_> using TAR = _TT<SIZE_, X_, D_, L_, R_>;
};


//================================================================================
template <class TIn, class T, T N, template <class, auto> class C>
struct getTInputTemplate<C<TIn, N>>
{
	static constexpr typeVar Type = typeVar::Arr;
	using Tin = TIn;
	using TSize = T;
	static constexpr T Size = N;
	template <class T1, auto N1> using TAR = C<T1, N1>;
};

template <class TIn, class T, T N, template <class, auto> class C>
struct getTInputTemplate<const C<TIn, N>>
{
	static constexpr typeVar Type = typeVar::Arr;
	using Tin = TIn;
	using TSize = T;
	static constexpr T Size = N;
	template <class T1, auto N1> using TAR = C<T1, N1>;
};

template <class TIn, class T, T N, template <class, auto> class C>
struct getTInputTemplate<const C<TIn, N>&>
{
	static constexpr typeVar Type = typeVar::Arr;
	using Tin = TIn;
	using TSize = T;
	static constexpr T Size = N;
	template <class T1, auto N1> using TAR = C<T1, N1>;
};


template <auto t> struct
getVInputTemplate :getTInputTemplate<decltype(t)> {};

template <const auto & t> struct
getVInputTemplateConstRef :getTInputTemplate<decltype(t)> {};

#define GETVINPUTTEMPLATE(x)  getTInputTemplate<decltype(x)>
#define GETVINPUTTEMPLATE_RES(x)  typename GETVINPUTTEMPLATE((x))::Tres
#define GETVINPUTTEMPLATE_IN(x)  typename GETVINPUTTEMPLATE((x))::Tin
#define GETVINPUTTEMPLATE_TSIZE(x)  typename GETVINPUTTEMPLATE((x))::TSize
#define GETVINPUTTEMPLATE_TTT(x) typename  GETVINPUTTEMPLATE((x))::template TAR
#define GETVINPUTTEMPLATE_SIZE(x)  GETVINPUTTEMPLATE((x))::Size


#pragma endregion

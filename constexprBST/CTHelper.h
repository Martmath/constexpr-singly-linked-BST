#pragma once
#include <iostream>
#include <array>
#include "OutSupport.h"
using namespace std;

template <auto F, size_t N, class T, const array<T, N> & Arr, size_t Index = 0, typename... Tpack>
constexpr auto cutUnsortedArray(Tpack&&... vpack)
{
	if constexpr (Index == N) return array<T, sizeof...(Tpack)>{ forward<Tpack>(vpack)... };
	else
	{
		if constexpr (F(Arr[Index]) != F(T{}))
			return cutUnsortedArray<F, N, T, Arr, Index + 1>(forward<Tpack>(vpack)..., Arr[Index]);
		else
			return cutUnsortedArray<F, N, T, Arr, Index + 1>(forward<Tpack>(vpack)...);
	}
}

template <auto F, template <class, auto> class TA, size_t N, class T, const array<T, N> & Arr, size_t Index = 0, typename... Tpack>
constexpr auto cutUnsortedArray(Tpack&&... vpack)
{
	if constexpr (Index == N) return TA<T, sizeof...(Tpack)>{ forward<Tpack>(vpack)... };
	else
	{
		if constexpr (F(Arr[Index]) != F(T{}))
			return cutUnsortedArray<F, N, T, Arr, Index + 1>(forward<Tpack>(vpack)..., Arr[Index]);
		else
			return cutUnsortedArray<F, N, T, Arr, Index + 1>(forward<Tpack>(vpack)...);
	}
}

template <auto F, template <class, auto> class TA,size_t N, class T, const TA<T, N> & Arr, size_t Index = 0, typename... Tpack>
constexpr auto cutSortedArray(Tpack&&... vpack)
{
	if constexpr (F(Arr[Index]) == F(T{}))
		return TA<T, sizeof...(Tpack)>{ forward<Tpack>(vpack)... };
	else
		return cutSortedArray<F,TA, N, T, Arr, Index + 1>(forward<Tpack>(vpack)..., Arr[Index]);
}
template <auto F, size_t N, class T, const array<T, N> & Arr, size_t Index = 0, typename... Tpack>
constexpr auto cutSortedArray(Tpack&&... vpack)
{
	if constexpr (F(Arr[Index]) == F(T{}))
		return array<T, sizeof...(Tpack)>{ forward<Tpack>(vpack)... };
	else
		return cutSortedArray<F, N, T, Arr, Index + 1>(forward<Tpack>(vpack)..., Arr[Index]);
}

//template <auto F, const auto & Arr,class Def= getTInputTemplate<decltype(Arr)> ,int Index = 0, typename... Tpack>
template <auto F, const auto & Arr,  size_t Index = 0, typename... Tpack>
constexpr auto cutSortedArray1(Tpack&&... vpack)
{
	using Def = getTInputTemplate<decltype(Arr)>;
	if constexpr (F(Arr[Index]) == F(Def::Tin{}))
		return  array<Def::Tin, sizeof...(Tpack)>{ forward<Tpack>(vpack)... };
	else
		return cutSortedArray1<F, Arr, Index + 1>(forward<Tpack>(vpack)..., Arr[Index]);
}



template < typename F, typename T, size_t N, template <class, auto> class TA>
constexpr void packArray(F f, TA<T, N>& mA, size_t* Z = nullptr)
{
	auto RR = f(T{});	auto BB = T{};	int k = -1;
	for (int i = 0; i < N; i++)
	{
		if ((f(mA[i]) == RR) && (k == -1)) 	k = i;
		else if ((f(mA[i]) != RR) && (k != -1))
		{
			mA[k] = mA[i]; k++;
		}
	}
	for (int i = k; i < N; i++)	mA[i] = BB;
	if (Z != nullptr) (*Z) = k;
}

template <typename T, size_t N, typename R, template <class, auto> class TA=array> constexpr void fillArrayFromList(TA<T, N>& mA, R t)
{
	if constexpr (isLT <R>::it() == ltLast)
	{
		mA[t.uid] = t.head;
	}
	else
	{
		if constexpr (isLT <R>::it() != ltZero)
		{
			mA[t.uid] = t.head;
			fillArrayFromList(mA, t.tail);
		}
	}
}

template <typename T, size_t N, typename R, template <class, auto> class TA = array> constexpr void fillFirstArrayFromList(TA<T, N>& mA, R t)
{
	if constexpr (isLT <R>::it() == ltLast)
	{
		mA[t.uid] = t.head;
	}
	else
	{
		if constexpr (isLT <R>::it() != ltZero)
		{
			mA[t.uid] = t.head;			
		}
	}
}

//No need & have max count control - use in environment with known max
template <int X, class H, class T> constexpr auto getListItem(CList<X,H,T> t, size_t N, size_t Nc)
{
	if constexpr (isLT<CList<X, H, T>>::isltLast())	return t.head;
	else
	{
		if constexpr (!isLT <CList<X, H, T>>::isltZero())
		{
			if (N == Nc)	return t.head;
			else			return getListItem(t.tail, N, Nc + 1);
		}
		else return H{};
	}
}

template <int X, class H, class T> constexpr auto getListFistItem(CList<X, H, T> t)
{
	if constexpr (isLT<CList<X, H, T>>::isltLast())	return t.head;
	else
	{
		if constexpr (!isLT <CList<X, H, T>>::isltZero())
		{
			return t.head;
		}
		else return H{};
	}
}

template<typename T>constexpr T wrp(int i) { return T{}; };

template<typename T, std::size_t... I> constexpr
auto fillArrayBaseV(std::index_sequence<I...>)
{
	return	array<T, sizeof...(I) > {wrp<T>(I)...};
}

template<template <class, auto> class TA,typename T, std::size_t... I> constexpr
auto fillArrayBaseV(std::index_sequence<I...>)
{
	return	TA<T, sizeof...(I) > {wrp<T>(I)...};
}

template <class T, template <class, auto> class TA, const auto & F, const auto & Tree,
	TA<TreeBase<decltype(Tree)>, getTInputTemplate<decltype(Tree)>::Size>(*DO)(decltype(F), decltype(Tree), T),
	T... C>
	struct TempWrapperArgs1 {
	static constexpr auto Temp = DO(F, Tree, C...);
	static constexpr auto Res = cutSortedArray
		<F, TA, Temp.size(), TreeBase<decltype(Tree)>, Temp>();
	constexpr auto operator () () const { return Res; };
};

/*TT instead FuncRes<DOfunc> -couse dsnt not work intellisense*/
template <class T, template <class, auto> class TA, const auto & F, const auto & Tree,
	//const auto& DO	
	TA<TreeBase<decltype(Tree)>, getTInputTemplate<decltype(Tree)>::Size>(*DO)(decltype(F), decltype(Tree), T, T)
	,
	T... C>
	struct TempWrapperArgs2 {
	static constexpr auto Temp = DO(F, Tree, C...);
	static constexpr auto Res = cutSortedArray
		<F, TA, Temp.size(), TreeBase<decltype(Tree)>, Temp>();
	constexpr auto operator () () const { return Res; };
};

/*template <class T, template <class, auto> class TA, const auto & F, const auto & Tree,
	  auto DO,
	T... C>
	struct TempWrapperArgs2 {
	static constexpr auto Temp = DO(F, Tree, C...);
	static constexpr auto Res = cutSortedArray
		<F, TA, Temp.size(), TreeBase<decltype(Tree)>, Temp>();
	constexpr auto operator () () const { return Res; };
};*/
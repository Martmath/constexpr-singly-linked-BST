#pragma once
#include <type_traits>
#include "BaseSupport.h"
#include "CheckType.h"


constexpr CZero Zero() {
	return CZero{};
};

template <class T, std::enable_if_t < !(numeric_limits<T>::is_specialized || hasZeroResT<T>{}) && (itOkForTree<T>{}) > * TT = nullptr > constexpr T Zero() {
	return T{};
};

template <class T, std::enable_if_t<numeric_limits<T>::is_specialized>* TT = nullptr>constexpr  T Zero() {
	return BASENUM;
};

template <class T, decltype(T::Zero) * TT = nullptr>constexpr  auto Zero() {
	return T::Zero;
};

template <template <auto, class, class, class> class T>constexpr auto Zero() {
	return T<0,CZero,CZero,CZero>{};};

template <template <auto, class, class, class,auto> class T>constexpr  auto Zero() {
	return T<0, CZero, CZero, CZero,0>{};
};

template <template <auto, class, class, class, auto,auto> class T>constexpr  auto Zero() {
	return T<0, CZero, CZero, CZero, 0,0>{};
};

template <template <auto, class, class, class, auto, auto,auto> class T>constexpr  auto Zero() {
	return T<0, CZero, CZero, CZero, 0, 0, 0>{};
};

template <template <auto, class, class, class, auto, auto, auto,auto> class T>constexpr  auto Zero() {
	return T<0, CZero, CZero, CZero, 0, 0, 0,0>{};
};

template <template <auto, class, class, class, auto, auto, auto, auto,auto> class T>constexpr  auto Zero() {
	return T<0, CZero, CZero, CZero, 0, 0, 0, 0,0>{};
};

template <template <auto,auto, class, class, class> class T>constexpr auto Zero() {
	return T<0,0, CZero, CZero, CZero>{};
};

template <template <auto,auto, class, class, class, auto> class T>constexpr  auto Zero() {
	return T<0,0, CZero, CZero, CZero, 0>{};
};

template <template <auto,auto, class, class, class, auto, auto> class T>constexpr  auto Zero() {
	return T<0,0, CZero, CZero, CZero, 0, 0>{};
};

template <template <auto,auto, class, class, class, auto, auto, auto> class T>constexpr  auto Zero() {
	return T<0,0, CZero, CZero, CZero, 0, 0, 0>{};
};

template <template <auto,auto, class, class, class, auto, auto, auto, auto> class T>constexpr  auto Zero() {
	return T<0,0, CZero, CZero, CZero, 0, 0, 0, 0>{};
};

template <template <auto,auto, class, class, class, auto, auto, auto, auto, auto> class T>constexpr  auto Zero() {
	return T<0,0, CZero, CZero, CZero, 0, 0, 0, 0, 0>{};
};

template <template <auto, auto,auto, class, class, class> class T>constexpr auto Zero() {
	return T<0, 0,0, CZero, CZero, CZero>{};
};

template <template <auto, auto,auto, class, class, class, auto> class T>constexpr  auto Zero() {
	return T<0, 0,0, CZero, CZero, CZero, 0>{};
};

template <template <auto, auto,auto, class, class, class, auto, auto> class T>constexpr  auto Zero() {
	return T<0, 0,0, CZero, CZero, CZero, 0, 0>{};
};

template <template <auto, auto,auto, class, class, class, auto, auto, auto> class T>constexpr  auto Zero() {
	return T<0, 0,0, CZero, CZero, CZero, 0, 0, 0>{};
};

template <template <auto, auto,auto, class, class, class, auto, auto, auto, auto> class T>constexpr  auto Zero() {
	return T<0, 0,0, CZero, CZero, CZero, 0, 0, 0, 0>{};
};

template <template <auto, auto,auto, class, class, class, auto, auto, auto, auto, auto> class T>constexpr  auto Zero() {
	return T<0, 0,0, CZero, CZero, CZero, 0, 0, 0, 0, 0>{};
};


template <class C>
constexpr bool sameZ() { return std::is_same<C, CZero>(); }

template <class C0, class C1 >
constexpr bool sameZ()
{
	return std::is_same<C0, CZero>() && std::is_same<C1, CZero>();
}

template <class C0, class C1, class C2, class C3, class... C>
constexpr bool sameZ()
{
	return std::is_same<C0, CZero>() && std::is_same<C1, CZero>() &&
		std::is_same<C2, CZero>() && std::is_same<C3, CZero>() &&
		((std::is_same<C, CZero>()) &&...);
}

template <auto... T>
constexpr bool sameZ() { return ((T == 0) &&...); }

template <class C0, class C1, class C2, auto... T>
constexpr bool sameZ()
{
	return ((std::is_same<C0, CZero>()) && (std::is_same<C1, CZero>()) && (std::is_same<C2, CZero>())) &&
		(sameZ<T...>());
};

constexpr bool checkZero(CZero t) { return true; };

template <class T, std::enable_if_t < ((!hasZeroResT<T>{}) && (itOkForTree<T>{})) > * TT = nullptr >
constexpr bool checkZero(T t)
{
	return (t == (Zero<T>()));
};

template <class T, std::enable_if_t <
((hasZeroResT<T>{}) && (itOkForTree<decltype(T::Zero)>{})) > * TT = nullptr >
constexpr bool checkZero(T wrapper, decltype(T::Zero) d)
{
	return d == Zero<decltype(T::Zero)>();
};
//=========================
template <template <auto, class, class, class> class T, auto T0, class C0, class C1, class C2, std::enable_if_t <
(!(itOkForTree<T<T0, C0, C1, C2>>{}) || (hasZeroResT<T<T0, C0, C1, C2>>{})) > * TT = nullptr >
constexpr bool checkZero(T<T0, C0, C1, C2> t) { return sameZ<C0, C1, C2, T0>(); };

template <template <auto, class, class, class, auto> class T, auto T0, class C0, class C1, class C2,
	auto T1, std::enable_if_t <
	(!(itOkForTree<T<T0, C0, C1, C2, T1>>{}) || (hasZeroResT<T<T0, C0, C1, C2, T1>>{})) > * TT = nullptr >
	constexpr bool checkZero(T<T0, C0, C1, C2, T1> t) { return sameZ<C0, C1, C2, T0, T1>(); };

template <template <auto, class, class, class, auto, auto> class T, auto T0, class C0, class C1, class C2,
	auto T1, auto T2, std::enable_if_t <
	(!(itOkForTree<T<T0, C0, C1, C2, T1, T2>>{}) || (hasZeroResT<T<T0, C0, C1, C2, T1, T2>>{})) > * TT = nullptr >
	constexpr bool checkZero(T<T0, C0, C1, C2, T1, T2> t) { return sameZ<C0, C1, C2, T0, T1, T2>(); };

template <template <auto, class, class, class, auto, auto, auto> class T, auto T0, class C0, class C1, class C2,
	auto T1, auto T2, auto T3, std::enable_if_t <
	(!(itOkForTree<T<T0, C0, C1, C2, T1, T2, T3>>{}) || (hasZeroResT<T<T0, C0, C1, C2, T1, T2, T3>>{})) > * TT = nullptr >
	constexpr bool checkZero(T<T0, C0, C1, C2, T1, T2, T3> t) { return sameZ<C0, C1, C2, T0, T1, T2, T3>(); };
//=============================
template <template <auto,auto, class, class, class> class T, auto T0, auto T01, class C0, class C1, class C2, std::enable_if_t <
(!(itOkForTree<T<T0,T01, C0, C1, C2>>{}) || (hasZeroResT<T<T0,T01, C0, C1, C2>>{})) > * TT = nullptr >
constexpr bool checkZero(T<T0,T01, C0, C1, C2> t) { return sameZ<C0, C1, C2, T0,T01>(); };

template <template <auto,auto, class, class, class, auto> class T, auto T0,auto T01, class C0, class C1, class C2,
	auto T1, std::enable_if_t <
	(!(itOkForTree<T<T0,T01, C0, C1, C2, T1>>{}) || (hasZeroResT<T<T0,T01, C0, C1, C2, T1>>{})) > * TT = nullptr >
	constexpr bool checkZero(T<T0,T01, C0, C1, C2, T1> t) { return sameZ<C0, C1, C2, T0, T1,T01>(); };

template <template <auto,auto, class, class, class, auto, auto> class T, auto T0, auto T01, class C0, class C1, class C2,
	auto T1, auto T2, std::enable_if_t <
	(!(itOkForTree<T<T0,T01, C0, C1, C2, T1, T2>>{}) || (hasZeroResT<T<T0,T01, C0, C1, C2, T1, T2>>{})) > * TT = nullptr >
	constexpr bool checkZero(T<T0,T01, C0, C1, C2, T1, T2> t) { return sameZ<C0, C1, C2, T0, T1, T2,T01>(); };

template <template <auto,auto, class, class, class, auto, auto, auto> class T, auto T0,auto T01, class C0, class C1, class C2,
	auto T1, auto T2, auto T3, std::enable_if_t <
	(!(itOkForTree<T<T0,T01, C0, C1, C2, T1, T2, T3>>{}) || (hasZeroResT<T<T0,T01, C0, C1, C2, T1, T2, T3>>{})) > * TT = nullptr >
	constexpr bool checkZero(T<T0,T01, C0, C1, C2, T1, T2, T3> t) { return sameZ<C0, C1, C2, T0, T1, T2, T3,T01>(); };
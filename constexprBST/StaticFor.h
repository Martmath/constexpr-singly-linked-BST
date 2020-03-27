#pragma once
template<template <auto> class F, std::size_t... I>
void staticForHelper(std::index_sequence<I...>) { (F<I>::Res(), ...); }

template<template <auto> class F, std::size_t N>
void staticFor() {
	staticForHelper<F>(std::make_index_sequence<N>());
}

template <size_t I> struct wrpLambda {};

#define STATICFORCREATE(LABEL,RUNF)\
constexpr auto LABEL##HELPER = [&]<std::size_t... I>(std::index_sequence<I...>)\
{	(RUNF(wrpLambda<I>{}), ...);};\
constexpr auto LABEL = [&]<size_t N>(wrpLambda<N> r)\
{	LABEL##HELPER(std::make_index_sequence<N>());};

#define STATICFORRUN(LABEL,COUNT)\
LABEL(wrpLambda<COUNT>{});

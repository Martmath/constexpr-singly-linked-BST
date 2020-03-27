#pragma once
#include <iostream>
#include <string>
#include <sstream>
template<class T>
struct is_str : std::false_type {};
template<> struct is_str<char*> : std::true_type {};
template<> struct is_str<wchar_t*> : std::true_type {};
template<> struct is_str<const char*> : std::true_type {};
template<> struct is_str<const wchar_t*> : std::true_type {};
template<> struct is_str<std::string> : std::true_type {};
template<> struct is_str<const std::string> : std::true_type {};
template<> struct is_str<std::wstring> : std::true_type {};
template<> struct is_str<const std::wstring> : std::true_type {};

template<size_t N, size_t M>
void setData(char(&a)[N], char(&b)[M], size_t Place, size_t Count = 1)
{
	std::copy(b, b + Count, a + Place);
}

template<size_t N, typename D>
void setData(char(&a)[N], D d, size_t Place, size_t Count, const char* F)
{
	char b[256]; size_t n = sprintf_s(b, F, d);
	if (n >= Count) std::copy(b, b + Count, a + Place);
	else std::copy(b, b + n, a + Place + ((Count - n) >> 1));
}

template<class T> struct is_c : std::false_type {};
template<> struct is_c<char> : std::true_type {};

template<size_t N, typename D, std::enable_if_t<(std::is_integral<D>::value) && (!is_c<D>::value)> * = nullptr>
void setData(char(&a)[N], D d, size_t Place, size_t Count) 
{ 
	setData(a, d, Place, Count, "%d"); 
}

template<size_t N, typename D, std::enable_if_t<std::is_floating_point<D>::value> * = nullptr>
void setData(char(&a)[N], D d, size_t Place, size_t Count) 
{
	 setData(a, d, Place, Count, "%f");
}

template<size_t N, typename D, std::enable_if_t<is_str<D>::value> * = nullptr>
void setData(char(&a)[N], D d, size_t Place, size_t Count) 
{ 
	setData(a, d, Place, Count, "%s"); 
}

template<size_t N, typename D, std::enable_if_t<is_c<D>::value> * = nullptr>
void setData(char(&a)[N], D d, size_t Place) 
{ 
	 setData(a, d, Place, 1, "%c"); 
}
//46146661/stdenable-if-t-to-separate-string-from-non-string-function-parameters

template<const auto & TR, size_t N, size_t L>// = 3>
auto printResStr() 
{
	constexpr auto OO = findLevelItemsaPlaces<TR, N>();
	
	//char abc[1000]; 
	size_t eee = firstShift(0, findNodeMLevel(TR),L) * 2 + L + 1;
	char abc[firstShift(0, findNodeMLevel(TR), L) * 2 + L + 1];//+2
	std::fill(begin(abc), end(abc) - 1, ' ');
	//abc[firstShift(0, findNodeMLevel(TR)) * 2 + L] = (char)0;
	abc[firstShift(0, findNodeMLevel(TR), L) * 2 + L ] = (char)0;
	//abc[firstShift(0, findNodeMLevel(TR)) * 2 + L+1] = (char)10;
	//abc[firstShift(0, findNodeMLevel(TR)) * 2 + L + 2] = (char)13;
	for (size_t j = 0; j < OO.size(); j++)
	{
		setData(abc, OO[j].second.I0, OO[j].first, L);
	}
	//strcat_s(abc, "\n");
	return string(abc);
}

template<template <auto> class F,  std::size_t... I>
auto forHelperStr(std::index_sequence<I...>) 
{
	/*char buffer[(sizeof...(I)+1)*findNodeMLevel(TR)) * 2 + 3 + 1 ];
	strcpy_s(buffer, F<I0>::Res());
	(strcat_s(buffer, F<I>::Res()), ...);
	return buffer;*/
	ostringstream ss;
	((ss << F<I>::res() << "\n"), ...);
	return ss.str();
}

template<template <auto> class F, std::size_t N>
auto forStr() 
{
	return forHelperStr<F>(std::make_index_sequence<N>());
}

template<const auto & TR, size_t N, size_t L = 3>
void printTree() 
{
	constexpr auto OO = findLevelItemsaPlaces<TR, N>();
	char abc[firstShift(0, findNodeMLevel(TR),L) * 2 + L + 1];
	std::fill(begin(abc), end(abc) - 1, ' ');
	abc[firstShift(0, findNodeMLevel(TR),L) * 2 + L] = (char)0;
	for (size_t j = 0; j < OO.size(); j++)
	{
		setData(abc, OO[j].second.I0, OO[j].first, L);
	}
	std::cout << abc << "\n";
}

template<size_t L = 3, class T, size_t N, template<class, auto> class TA = array>
void printa(const TA<T, N>& OO) {
	//constexpr auto OO = findFirstLevel3<TR, N>();
	char abc[N * L + (N - 1) * (L + 1)];
	std::fill(begin(abc), end(abc) - 1, ' ');
	abc[N * L + (N - 1) * (L + 1) - 1] = (char)0;

	for (size_t j = 0; j < OO.size(); j++)
	{
		setData(abc, OO[j].I0, j * (L + 1), L);
	}
	std::cout << abc << "\n";
}

template <size_t I>size_t wrp_() { return I; };
template <size_t I, const auto & K, size_t L = 3> struct TwrpOutHelperG
{
	//static template<const auto & TR> 
	static constexpr void Res() 
	{      
		printTree<K, I, L>();
	};
};

template <const auto & K, size_t L = 3> struct TwrpOutG
{
	template<size_t I>
	using R = TwrpOutHelperG<I, K, L>;
};

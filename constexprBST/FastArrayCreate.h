#pragma once


struct q { int w = 6; int e = 3; } v;
template <typename Tpack0, typename... Tpack> constexpr auto arrayCreate(Tpack&& ... vpack) {
	return std::array<Tpack0, sizeof...(Tpack)>{Tpack0{ vpack }...};
}

#include "MacrosLoop.h"

#if 0
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>

#if __has_include(<boost/preprocessor/seq/for_each.hpp>) //in msvs check file existing.....not include!!!!! kcuf

#define BOOST_PP_ARRAYCREATEHELPER(r, data, elem) ,data{ PASS_PARAMETERS(elem)}
#define BOOST_PP_ARRAYCREATE(ff,f,F,...) ff<f>(f{PASS_PARAMETERS(F)}BOOST_PP_SEQ_FOR_EACH(\
 BOOST_PP_ARRAYCREATEHELPER, f, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))) 

static constexpr auto checkBoostArrayCreate = BOOST_PP_ARRAYCREATE(arrayCreate, q, (7, 6), (4, 3));
#endif
#endif
//#define TRM(TYPE,ARG) ,TYPE{ARG} 
#define ARRAYCREATEHELPER(r) ,q{ r} 
#define ARRAYCREATE(ff,f,F,...) EXPAND(ff<f>(f{ \
 PASS_PARAMETERS(F) } FOR_EACH(ARRAYCREATEHELPER,__VA_ARGS__)));
//https://stackoverflow.com/questions/24481810/how-to-remove-the-enclosing-parentheses-with-macro
//https://stackoverflow.com/questions/1872220/is-it-possible-to-iterate-over-arguments-in-variadic-macros

//https://stackoverflow.com/questions/24481810/how-to-remove-the-enclosing-parentheses-with-macro
//https://stackoverflow.com/questions/1872220/is-it-possible-to-iterate-over-arguments-in-variadic-macros


template <typename R, typename ...R1> constexpr auto arrCreate(R t0, R1... t) {
	return array<R, sizeof...(R1) + 1>{t0, t...};
}

//=======================================
static constexpr auto f = ARRAYCREATE(arrayCreate, q, (74, 6), (7, 6), (4, 3));
static constexpr auto f3 = arrayCreate<q>((74, 6), (7, 6), (4, 3));

static constexpr auto S1s = arrayCreate<Test>((4), (99, 99), (3));
//static constexpr 
auto fdd = ARRAYCREATE(arrayCreate, q, (74, 6), (7, 6), (4, 3));
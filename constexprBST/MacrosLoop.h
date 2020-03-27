


#define _Args(...) __VA_ARGS__
#define STRIP_PARENS(X) X
#define PASS_PARAMETERS(X) STRIP_PARENS( _Args X )
//BOOST_PP_SEQ_FOR_EACH_R(r, macro, data, seq)
#define EXPAND(x) x
#define CONCATENATE(arg1, arg2) CONCATENATE1(arg1, arg2)
#define CONCATENATE1(arg1, arg2) arg1##arg2
#define FOR_EACH_1(what, x, ...) what(PASS_PARAMETERS(x))

#define FOR_EACH_2(what, x, ...)\
what(PASS_PARAMETERS(x)) FOR_EACH_1(what, __VA_ARGS__)

#define FOR_EACH_3(what, x, ...)\
what(PASS_PARAMETERS(x)) FOR_EACH_2(what, __VA_ARGS__)

#define FOR_EACH_4(what, x, ...)\
what(PASS_PARAMETERS(x)) FOR_EACH_3(what, __VA_ARGS__)

#define FOR_EACH_5(what, x, ...)\
what(PASS_PARAMETERS(x)) FOR_EACH_4(what, __VA_ARGS__)

#define FOR_EACH_6(what, x, ...)\
what(PASS_PARAMETERS(x)) FOR_EACH_5(what, __VA_ARGS__)

#define FOR_EACH_7(what, x, ...)\
what(PASS_PARAMETERS(x)) FOR_EACH_6(what, __VA_ARGS__)

#define FOR_EACH_8(what, x, ...)\
what(PASS_PARAMETERS(x)) FOR_EACH_7(what, __VA_ARGS__)

#define FOR_EACH_9(what, x, ...)\
what(PASS_PARAMETERS(x)) FOR_EACH_8(what, __VA_ARGS__)

#define FOR_EACH_10(what, x, ...)\
what(PASS_PARAMETERS(x)) FOR_EACH_9(what, __VA_ARGS__)

#define FOR_EACH_11(what, x, ...)\
what(PASS_PARAMETERS(x)) FOR_EACH_10(what, __VA_ARGS__)

#define FOR_EACH_12(what, x, ...)\
what(PASS_PARAMETERS(x)) FOR_EACH_11(what, __VA_ARGS__)

#define FOR_EACH_13(what, x, ...)\
what(PASS_PARAMETERS(x)) FOR_EACH_12(what, __VA_ARGS__)

#define FOR_EACH_14(what, x, ...)\
what(PASS_PARAMETERS(x)) FOR_EACH_13(what, __VA_ARGS__)

#define FOR_EACH_15(what, x, ...)\
what(PASS_PARAMETERS(x)) FOR_EACH_14(what, __VA_ARGS__)

#define FOR_EACH_ARG_N(\
 _1, _2, _3, _4, _5, \
_6, _7, _8, _9, _10, \
_11, _12, _13, _14, _15, \
 \
N,...) N
#define FOR_EACH_NARG(...) EXPAND(FOR_EACH_ARG_N(__VA_ARGS__, \
 15, 14, 13, 12, 11, \
10, 9, 8, 7, 6, \
5, 4, 3, 2, 1, \
0))
#define FOR_EACH(what,...) \
EXPAND(CONCATENATE(FOR_EACH_,FOR_EACH_NARG(__VA_ARGS__))(what, __VA_ARGS__)) 

 /* static void Foo(){  std::cout << "Hello World0!\n"; 
 std::cout << "Hello World1!\n"; 
 std::cout << "Hello World2!\n"; 
 std::cout << "Hello World3!\n"; 
 std::cout << "Hello World4!\n"; 
 }
static void Foo1 () {  std::cout << "Hello World0!\n";  std::cout << "Hello World1!\n";  std::cout << "Hello World2!\n";  } */ /* //PopIndent(); 
9876543210 */ 

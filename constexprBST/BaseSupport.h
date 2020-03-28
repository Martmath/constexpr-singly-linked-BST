#pragma once

//https://stackoverflow.com/questions/41301536/get-function-return-type-in-template 
//https://stackoverflow.com/questions/17287778/how-do-i-use-the-class-name-inside-the-class-itself-as-a-template-argument 
//https://stackoverflow.com/questions/31762958/check-if-class-is-a-template-specialization


#include <array>
using namespace std;

#define BASENUM 99//INT_MAX-5 -- too long for check err

#define I_0 I0 //IntData0
#define I_1 G1 //IntData1
#define I_S IS //StrData

struct CZero 
{

	friend ostream& operator<<(ostream& os, const CZero& dt) 
	{
		os << "invenisti continnum mea iuvenes amicus";
		return os;
	}
};

#pragma region CheckVCVersion
#if   defined(_MSVC_LANG) &&(_MSVC_LANG<=201703)
static_assert(false, "correct just for latest verion c++ msvc. Set /std:c++latest standard");
#endif

//VS2019 16.2.3 - __cplusplus >= 201703L - requires /Zc:__cplusplus or ==199711
#if   defined(_MSC_VER) &&(_MSC_VER >=1922)&&defined(_MSVC_LANG) &&(_MSVC_LANG>201703)  
#define SpaceShipOperator
#endif
#pragma endregion


#ifdef SpaceShipOperator
#include <compare>
//https://habr.com/ru/company/pvs-studio/blog/465575/
#endif








#pragma region Comparators

	template<typename TIn, typename TOut = int> using TgetKeyFunc = const TOut(const TIn&);
	template<typename TIn>constexpr const bool getKeyFuncZeroDefaultErr(const TIn& t) 
	{
		static_assert(false, "no init same to default checker");
		return false;
	};

	template<typename TIn>constexpr const bool getKeyFuncZeroDefault(const TIn& t)
	{		
		return t == TIn{};
	};
	template<typename TRes, typename TIn> constexpr TgetKeyFunc<TIn, TRes> getKeyFunc0;
	template<typename TRes, typename TIn>constexpr const TRes getKeyFunc0(const TIn& t) { return t.I_0; };
	

	template<typename TRes, typename TIn> constexpr TgetKeyFunc<TIn, TRes> getKeyFunc1;
	template<typename TRes, typename TIn>constexpr const TRes getKeyFunc1(const TIn& t) { return t.I_1; };

	template<typename TRes> constexpr TgetKeyFunc<TRes, TRes> getKeyFunc2;
	template<typename TRes> constexpr const TRes getKeyFunc2(const TRes& t) { return t; };
#pragma endregion



#pragma endregion

//template<typename Tin, typename TKK = int> using _t_ = const TKK(*)(const Tin&);
//template<typename T, bool Right = true>
//using TLimited = std::enable_if_t<(Right == (numeric_limits<T>::is_specialized))>*;


/*
template<class T, class R = void> struct enable_if_type { typedef R type; };
template<class T> using enable_if_type_t = typename enable_if_type<T>::type;
template<class T, class Enable = void> struct conditionsArrayTemp : false_type {};
template <class T> using TSamePointer = const bool (T::*)(T) const;
template <class T> using TSamePointer1 = const bool (T::*)(T, T);

template<class T, class Enable = void> struct conditionsSameTemp : false_type {};
template<class T>struct conditionsSameTemp
<T, enable_if_type_t<decltype((TSamePointer<T>) & T::operator==)>> :true_type {};
template<class T> constexpr bool conditions1 = conditionsSameTemp<T>::value;
template<class T, class Enable = void> struct conditionsSameTemp1 : false_type {};
template<class T>struct conditionsSameTemp1
<T, enable_if_type_t <decltype((TSamePointer1<T>) & T::operator==)>> :true_type {};
template<class T> constexpr bool conditions11 = conditionsSameTemp1<T>::value;
//https://stackoverflow.com/questions/6534041/how-to-check-whether-operator-exists
/*namespace CHECK
{
	template<typename T, typename Arg> false_type
		operator == (const T&, const Arg&);

	template<typename T, typename Arg = T> constexpr bool TT() {
		return std::is_same_v<decltype(declval<T>() == declval<Arg>()), bool>;
	}
}*/
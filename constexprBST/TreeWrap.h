#pragma once
#include <iostream>
#include <array>
#include "TreeCreatorOld.h"
#include "TreeCreator.h"
#include "TreeCreatorNode.h"
#include "CTHelper.h"
#include "CTFindBetween.h"
#include "CTCurrentNode.h"
#include "CTFindAll.h"
#include "CTFindFirstList.h"
#include "CTLevel.h"
#include "CTOutPlaces.h"
#include "OutSupport.h"
#include "TreeClone.h"
#include "TreePrint.h"
#include "StaticFor.h"
#include "FastArrayCreate.h"
#include "ExtArray.h"

enum  varCreate { autoAlgCreate = 0, autoAlgCreateOld, forNode,trCreator,trCreatorLim, trCreatorOld, trCreatorLimOld,trZero} ;

template<const auto & F, const auto & Tree, auto C, template <class, auto> class TA = array> struct TreeWrapNode
{
	static constexpr  auto nodeArray = ::findCurrentNode<F, Tree, C, TA>();

	static constexpr  auto res() 
	{
		if constexpr  (getTInputTemplate<decltype(nodeArray)>::Size)
			return TreeWrap<F, nodeArray, varCreate::forNode,3,CZero,TA>{};
		else 
			return TreeWrap<F, nodeArray, varCreate::trZero, 3, CZero, TA>{};
	}
};

template<const auto & F, const auto & mA, varCreate Var = varCreate::autoAlgCreate, size_t LenOut=3, 
class TUnpackArray = getTInputTemplate<clearType_t<decltype(mA)>>,
template <class, auto> class TmA = TUnpackArray::template TAR
	//typename  getTInputTemplate<clearType_t<decltype(U_)>>::template TAR - Kcuf swimming here
> 
struct TreeWrap 
{
	//static constexpr auto lenOut = LenOut;
	using TT = clearType_t<decltype(F)>;
	using TRes = typename getTInputTemplate<TT>::Ctres;
	using TIn = typename getTInputTemplate<TT>::Ctin;

	template <class T1, auto N1> using TAtemp = TmA<T1, N1>;

	static constexpr TAtemp<TIn, 0> AZero{};


	#define CHECKZERO(Ret) if constexpr (sameZ<decltype(Tree)>()) return AZero; else {return Ret;}
	#undef CHECKZERO
	
	template<const auto F>	static constexpr  auto
	RunCheckZero()
	{
		if constexpr (sameZ<decltype(Tree)>())		return AZero;
		else { return F(); }
	};

	//Creating a tree using two algorithms. For numeric types and types with comparison operators.
	static constexpr auto getTree()
	{
		if constexpr (NL<TRes>)
		{
			return treeCreatorLim<F, TIn, mA.size(), TAtemp, mA>();
		}
		else		
		{
			if constexpr (itOkForTree<TRes>{})
			{
				return treeCreator<F, TIn, mA.size(), TAtemp, mA>();
			}
			else
			{
				static_assert(false, "bad type getKeyFunc");
			}
		}
	}

	//Creating a tree using two algorithms. For numeric types and types with comparison operators.
	static constexpr auto getTreeOld()
	{
		if constexpr (NL<TRes>)
		{
			return treeCreatorLimOld<F, TIn, mA.size(), TAtemp, mA>();
		}
		else
		{
			if constexpr (itOkForTree<TRes>{})
			{
				return treeCreatorOld<F, TIn, mA.size(), TAtemp, mA>();
			}
			else
			{
				static_assert(false, "bad type");
			}
		}
	}

	//enum  varCreate { autoAlgCreate = 0, autoAlgCreateOld, forNode, trCreator, trCreatoLim, trCreatorOld, trCreatorLimOld };
	static constexpr auto selector()
	{
		if constexpr (getTInputTemplate<clearType_t<decltype(mA)>>::Type == typeVar::Tree)
		{
			//dont forget set TmA by Hand!!!
			return mA;
		}
		else 
		{
			if constexpr (Var == varCreate::autoAlgCreate)
			{
				return getTree();
			}
			else
			{
				if constexpr (Var == varCreate::autoAlgCreateOld)
				{
					return getTreeOld();
				}
				else
				{
					if constexpr (Var == varCreate::forNode)
					{
						//constexpr int a = -1000;
						//constexpr int b = 1000;
						return treeCreatorNode<F, mA>();// , a, b > ();
					}
					else
					{
						if constexpr (Var == varCreate::trCreator)
						{
							return treeCreator<F, TIn, mA.size(), TAtemp, mA>();
						}
						else
						{
							if constexpr (Var == varCreate::trCreatorLim)
							{
								return treeCreatorLim<F, TIn, mA.size(), TAtemp, mA>();
							}
							else
							{
								if constexpr (Var == varCreate::trCreatorOld)
								{
									return treeCreatorOld<F, TIn, mA.size(), TAtemp, mA>();
								}
								else
								{
									if constexpr (Var == varCreate::trCreatorOld)
									{
										return treeCreatorLimOld<F, TIn, mA.size(), TAtemp, mA>();
									}
									else
									{
										return CZero{};
									}
								}
							}
						}
					}
				}
			}
		}
	}
			   	  	   
	static constexpr auto Tree = selector();
	// treeCreator<F, TIn, mA.size(), TAtemp, mA>();   
	//getTreeOld();//treeCreatorOld//treeCreatorNumLimOld
	//<F, TIn, mA.size(), TAtemp, mA>();//getTree();
	//treeCreatorLim<F, TIn, mA.size(), TAtemp, mA>();
	//TempTree with extended node type
	static constexpr auto TreeTest = CreateTreePlace(Tree, LenOut);

	template <size_t I>size_t wrp_() { return I; };
	template <size_t I, const auto & K, size_t L = 3> struct TwrpOutHelper
	{
		static constexpr void res() 
		{
			printTree<K, I, L>();
		};
	};

	template <const auto & K, size_t L = LenOut> struct TwrpOut
	{
		template<size_t I>
		using R = TwrpOutHelper<I, K, L>;
	};

	template <size_t I, const auto & K, size_t L = LenOut> struct TwrpOutHelperStr
	{
		static constexpr auto  res() {
			return printResStr<K, I, L>();
		};
	};

	template <const auto & K, size_t L = LenOut> struct TwrpOutStr
	{
		template<size_t I>
		using R = TwrpOutHelperStr<I, K, L>;
	};

	static auto printStr()
	{
		return forStr<TwrpOutStr<TreeTest, LenOut>::template R, findNodeMLevel(TreeTest)>();
	};
	
	static void print()
	{
		staticFor<TwrpOut<TreeTest, LenOut>::template R, findNodeMLevel(TreeTest)>();
	};	

	//find values using the initializing key function from the current class instance &
	//the left and right val. Boundary include
	template<TRes l, TRes r, template <class, auto> class TA = TAtemp>
	static constexpr  auto findBetween() 
	{
		if constexpr (sameZ<decltype(Tree)>()) 	return AZero;
		else	return ::findBetween<F, Tree, l, r, TA>();
	};

	//find all values using the predicate (F)
	template<const auto F, template <class, auto> class TA = TAtemp>
	static constexpr  auto findAll()	
	{
		if constexpr (sameZ<decltype(Tree)>()) 	return AZero;
		else	return ::findAll<F, Tree, TA>();		
	};

	//get a values from node use the specified key
	template<TRes C, template <class, auto> class TA = TAtemp>
	static constexpr  auto findCurrentNode()
	{
		if constexpr (sameZ<decltype(Tree)>()) 	return AZero;
		else	return ::findCurrentNode<F, Tree, C, TA>();
	};

	//get a subtree for the specified key
	template<TRes C, template <class, auto> class TA = TAtemp>
	static constexpr  auto findCurrentNodeTree() 
	{
		return   TreeWrapNode<F, Tree, C,TA>::res(); //check to nullable inside res()
		//treeCreatorNode<F, ::findCurrentNode<F, Tree, C, TA>()>();
		//return ::findCurrentNode<F, Tree, C, TA>();
	};

	//get first items from nodes(list) at some level (C - Level, L - Key Length)
	template<TRes C, template <class, auto> class TA = TAtemp>
	static constexpr  auto findLevelFirst() 
	{
		if constexpr (sameZ<decltype(Tree)>()) 	return AZero;
		else	return ::findLevelFirst<Tree, C, TA>();
	};

	//get values from node list
	template<TRes C, template <class, auto> class TA = TAtemp>
	static constexpr  auto findLevelAll() 
	{
		if constexpr (sameZ<decltype(Tree)>()) 	return AZero;
		else	return ::findLevelA<F, Tree, C, TA>();
	};

	//get values from node list
	template<TRes C, template <class, auto> class TA = TAtemp>
	static constexpr  auto findFirstList() 
	{
		if constexpr (sameZ<decltype(Tree)>()) 	return AZero;
		else	return ::findFirstList<F, Tree, C, TA>();
	};

	//get place first items from nodes at some level (C - Level, L - Key Length)
	template<TRes C, TRes L=3, template <class, auto> class TA = TAtemp>
	static constexpr  auto findLevelShift() 
	{
		if constexpr (sameZ<decltype(Tree)>()) 	return AZero;
		else	return ::findLevelPlacesNOTUseHelperClass<Tree, C,L, TA>();
	};

	//get places and first item from nodes at some level (C)
	template<TRes C, template <class, auto> class TA = TAtemp>
	static constexpr  auto findLevelItemsaPlaces() 
	{
		if constexpr (sameZ<decltype(Tree)>()) 	return AZero;
		else	return ::findLevelItemsaPlaces<TreeTest, C, TA>();
	};

	//get tree max Level
	static constexpr  size_t maxLevel() 
	{
		if constexpr (sameZ<decltype(Tree)>()) 	return 0;
		else	return ::findNodeMLevel(Tree);
	};

	//get tree nodes count
	static constexpr  size_t nodeCount() 
	{
		if constexpr (sameZ<decltype(Tree)>()) 	return 0;
		else	return ::findNodeCount(Tree);
	};

	//check tree contains item by key
	static constexpr  bool contains(TRes key) 
	{ 
		if constexpr (sameZ<decltype(Tree)>()) 	return 0;
		else	return ::contains<F>(Tree,key); 
	};

	friend ostream& operator<< <F, mA, Var, LenOut, TUnpackArray, TmA> (ostream& os, const TreeWrap<F, mA,Var, LenOut, TUnpackArray, TmA>& a);
	/*{
		os << a.print_();
		return os;
	};*/
};

template<const auto & F, const auto & mA, varCreate Var = varCreate::autoAlgCreate, size_t LenOut = 3, 
class TUnpackArray = getTInputTemplate<clearType_t<decltype(mA)>>,
template <class, auto> class TmA = TUnpackArray::template TAR>
ostream& operator<<(ostream& os, const TreeWrap<F, mA, Var, LenOut, TUnpackArray, TmA>& a)
{
	os << a.printStr();
	return os;
};

template<const auto & F, const auto & mA, varCreate Var, size_t Len,
	class T, size_t N, template <class, auto> class TA>
	constexpr auto wrapperTreeFactoryHelper(const TA<T, N>& u)
{
	/*if constexpr (getTInputTemplate<clearType_t<decltype(mA)>>::Type == typeVar::Tree)
	{ return TreeWrap<F, mA, Var, Len, CZero, array>{};}	else*/
	return TreeWrap<F, mA, Var, Len, CZero, TA>{};
};

template<const auto & F, const auto & mA, varCreate Var, size_t Len, class T, template <class, auto> class TA = array>
constexpr auto wrapperTreeFactoryHelper(const T& u)
{
	return TreeWrap<F, mA, Var, Len, CZero, array>{};
};

template<const auto & F, const auto & mA,varCreate Var,size_t Len, template <class, auto> class TA = array>
constexpr auto wrapperTreeFactory() 
{
	return TreeWrap<F, mA, Var, Len, CZero, TA>{};
	
	//return wrapperTreeFactoryHelper<F, mA, Var, Len>(mA);
};




template <const auto & _F, const auto & _mA, auto _Var, size_t _LenOut,
class _TUnpackArray, template <class, auto> class _TmA>
struct getTInputTemplate<TreeWrap<_F, _mA, _Var, _LenOut, _TUnpackArray, _TmA>>
{
	static constexpr auto F = _F;
	static constexpr auto mA = _mA;
	static constexpr auto Var = _Var;
	static constexpr auto LenOut = _LenOut;
	template <class T1, auto N1> using TAR = _TmA<T1, N1>;
};

template<const auto & T0, const auto &... T> struct arrPack
{
	static constexpr array<Test, sizeof...(T) + 1> WW{ T0,T... };

	static constexpr auto tmpArr() {
		return array<Test, sizeof...(T) + 1>{ T0, T... };
	}

	template<const auto & F> static constexpr auto TrWrap() {
		return TreeWrap<F, WW, varCreate::autoAlgCreate, 3, getTInputTemplate<clearType_t<decltype(WW)>>>();
	}
};

static constexpr Test E0{ 7 }; static constexpr Test E1{ 12 }; static constexpr Test E2{ 15 };
static constexpr Test E3{ 5 }; static constexpr Test E4{ 3 }; static constexpr Test E5{ 0 };

static constexpr arrPack<E0, E1, E2, E3, E4, E5> checkArrWrp{};
static constexpr auto packArr = decltype(checkArrWrp)::tmpArr();
constexpr auto vv1 = decltype(checkArrWrp)::TrWrap<getKeyFn>();
using VV1 = decltype(vv1);
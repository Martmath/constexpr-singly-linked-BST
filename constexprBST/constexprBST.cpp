
#pragma once
#include "TreeWrap.h"
#include "ExtArray.h"
#include "FastArrayCreate.h"

#define WW_
#define XX_
#include "MacrosTT.h"

using namespace std;

constexpr int t0 = IFDEF2(WW_, 5, 7);
constexpr int t1 = IFDEF4(XX_, YY_, 4, 2, 3, 9);

static constexpr auto U = arrCreate(Test{ 0 }, Test{ 4 }, Test{ }, Test{ 3 }, Test{ }, Test{ 8 },
	Test{ 7,0 }, Test{ }, Test{ 6 }, Test{ 11 }, Test{ 14,0 }, Test{ }, Test{ 1 }, Test{ 7,1 }, Test{ 16,0 },
	Test{ 14,1 }, Test{ }, Test{ 16,1 }, Test{ 14,2 }, Test{ 7,2 }, Test{ 16,2 }, Test{ 2 }, Test{ 1 }, Test{ 54 });

static constexpr auto UT = treeCreatorLim<getKeyFunc0 <int, decltype(U)::value_type>, decltype(U)::value_type, U.size(),array, U>();

auto UT1 = treeCreatorLim//Old
<getKeyFunc0 <int, decltype(U)::value_type>, decltype(U)::value_type, U.size(), array, U>();
auto UT2 = treeCreator//Old
<getKeyFunc0 <int, decltype(U)::value_type>, decltype(U)::value_type, U.size(), array, U>();

//get place tree items in screen
static constexpr auto K = CreateTreePlace(UT, 3);
static constexpr auto XX = findAll<getKeyAll, UT, _array>();

using Tr = TreeWrap<getKeyFn, XX, varCreate::autoAlgCreate, 5, CZero, _array>;
using TT = TreeWrap<getKeyFn, XX, varCreate::trCreator, 9, CZero, _array>;
// decltype(wrapperTreeFactory<getKeyFn,XX, varCreate::autoAlgCreate,3>());

static constexpr auto Tr1 = Tr::findCurrentNode<7>();
using Tr2 = decltype(wrapperTreeFactory<getKeyFn, Tr1, varCreate::forNode, 3>());
using Tr3 = decltype(wrapperTreeFactory<getKeyFn, Tr2::Tree, varCreate::forNode, 3>());
using Tr4 = decltype(Tr3::findCurrentNodeTree<2>());

static constexpr auto Kw = firstShift(0, findNodeMLevel(UT)) * 2 + 3;




int main()
{	
	//std::cout << "checked tree0:\n" << VV1{};

	std::cout <<"checked tree:\n" << Tr{};
	constexpr auto z0 = Tr::findCurrentNodeTree<2>();
	std::cout << "get node by key=2:\n" << Tr::findCurrentNodeTree<2>();
	
	std::cout << "checked tree2:\n" << TT{};
	constexpr auto z1 = TT::findCurrentNodeTree<3>();
	std::cout << "get node by key=3:\n" << z1;
	
	constexpr auto z2 = Tr::findBetween<3, 9>();
	std::cout <<"items between 3 & 9 including border val: "<< z2;
	constexpr auto z3 = Tr::findAll<getKeyAll>();
	std::cout << "all tree value: " << z3;

	constexpr auto z4 = Tr::findAll<getKey2_7_14>();
	std::cout << "find items by 2,7,14 key: " << z4;

	constexpr auto z5 = Tr::findCurrentNode<14>();
	std::cout << "value from node ans subnode with key 14: " << z5;
	constexpr auto z6 = Tr::findFirstList<7>();
	std::cout << "value from node with key 7: " <<z6;
	constexpr auto z7 = Tr::findLevelAll<3>();
	std::cout << "value from level 3: " << z7;
	constexpr auto z8 = Tr::maxLevel();
	std::cout << "maxLevelCount: " << z8 << "\n";
	constexpr auto z9 = Tr::nodeCount();
	std::cout << "nodeCount (see all tree value): " << z9 <<"\n";
	constexpr auto z10 = Tr::contains(15);
	std::cout << "contain 15: " << (z10 ? "true":"false") << "\n";
	constexpr auto z11 = Tr::contains(8);
	std::cout << "contain 8: " << (z11 ? "true" : "false") << "\n";

	//return;

#pragma region check view Tree
	constexpr auto Z0 = Tr::findLevelShift<3, 5>();
	constexpr auto Z1 = Tr::findLevelItemsaPlaces<3>();
	//wrapperTreeFactory<getKeyFn, MM, varCreate::autoAlgCreate, 3>();VV::print();

	auto PRINTK = [&]<size_t T>(wrpLambda<T> w)
	{
		printTree<Tr::TreeTest, T, getTInputTemplate<Tr>::LenOut>();
	};	
	
	STATICFORCREATE(LABEL, PRINTK);
	STATICFORRUN(LABEL, findNodeMLevel(Tr::TreeTest));

	constexpr auto R1 = (7 - 3) >> 1;
	auto f = []<typename T>(T a, T b)->T {
		std::cout << 9 << "\n"; return a + b;
	};/*auto r1 = f(5,6);*/

	auto f0 = [&]<size_t T>(wrpLambda<T> w)->size_t {
		std::cout << T << "\n"; 
	};

	staticFor<TwrpOutG<K, 9>::template R, findNodeMLevel(K)>();

	constexpr auto Vl1 = findLevelFirst<UT, 3>();
	printa<5>(Vl1);
#pragma endregion
	constexpr auto H0 = findLevelItemsaPlaces<K, 3>();
	constexpr auto H1= findNodeMLevel(UT);
	constexpr auto H2 = CreateTreePlace(UT);
	//int XD=ceil((E + .0) / 2);
	std::cout << H2.list.head.I0 << std::endl;
	constexpr auto H3 = findNodeMLevel(UT);
	constexpr
	auto H4 =
		findLevelPlaceNOTUseHelperClass(UT, 3, 1, firstShift(0, findNodeMLevel(UT)), findNodeMLevel(UT));
	constexpr
	auto H5 = findLevelPlacesNOTUseHelperClass<UT , 3,1>();
	constexpr
		auto H6 = firstShift(0, findNodeMLevel(UT), 1);	
	//constexpr
		auto H7 = findLevelPlaceNOTUseHelperClass(UT, 3, 0, firstShift(0, findNodeMLevel(UT), 1), findNodeMLevel(UT), 1);
	auto H8 = findLevelPlacesUseHelperClass<K, 3>();
	constexpr	
		auto H9 =	findLevelCount(K, 4);
	constexpr	
		auto H10 = findLevelFirst<K, 4>();
	constexpr	
		auto H11 = findLevelPlacesUseHelperClass<K, 4>();	
	constexpr	
		auto H12 = findFirstLevelArray<array>(getKeyFn, UT, 1);	
	constexpr	
		auto H13 =	findFirstLevelA<getKeyFn, UT, 0, _array>();
	constexpr	
		auto H14 =	findLevelCount(UT, 2);
	constexpr	
		auto H15 = getTInputTemplate<decltype(H2.right.left)>::LEVEL;
	constexpr	
		array<Test,7> H16 = findBetween<getKeyFn, UT, 3, 9,array>();		
	constexpr
		auto H17 = FINDBETWEEN(getKeyFn, UT, 3, 9);	
	constexpr 
		auto H18 = findAll<getKey2_7_14, UT,_array>();
	constexpr 
		auto H19 = findAllA<getKeyFn, getKey2_7_14, UT,_array>();
	constexpr 
		auto H20 = findBetweenA<getKeyFn, UT, 3, 9>();	
	constexpr
		auto H21 = findBetweenA<typename getTInputTemplate<decltype(getKeyFn)>::Tres,getKeyFn, UT, 3, 9,_array>();
	constexpr
		auto H22= FINDBETWEENA(getKeyFn, UT, 3, 9,_array);		
	constexpr
		auto H23 = FINDCURRENTNODEA(getKeyFn, UT, 14);
	constexpr
		auto H24 = FINDCURRENTNODE(getKeyFn, UT, 14,array);
	constexpr
		auto H25 = FINDFIRSTLISTA(getKeyFn, UT, 7, array);
	constexpr
		auto H26 = findFirstListCount<getKeyFn>(UT, 14);
	constexpr
		auto H27 = findFirstListItem<getKeyFn>(UT,5, 7);
	constexpr
		auto H28 = FINDFIRSTLIST(getKeyFn, UT, 14);		
	auto H29 = treeCreatorLim<getKeyFunc0 <int, decltype(U)::value_type>, decltype(U)::value_type, U.size(),array, U>();		
} 
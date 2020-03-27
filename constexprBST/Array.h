#pragma once

//st/q/41660062/how-to-construct-an-stdarray-with-index-sequence
#include <iostream>
#include <array>
#include "OutSupport.h"
using namespace std;

namespace cxArray
{
	static constexpr size_t one = 1;
#pragma region sort array
	//https://stackoverflow.com/questions/19559808/constexpr-initialization-of-array-to-sort-contents 
	template<class T>
	constexpr void _swap(T& l, T& r)
	{
		T tmp = move(l);
		l = move(r);
		r = move(tmp);
	};

	template <typename T, size_t N>
	constexpr void sortArray(array<T, N>& a, array<size_t, N>& aIdx, size_t lt, size_t rt)
	{
		if (lt < rt)
		{
			size_t m = lt;
			for (T i = lt + 1; i < rt; i++)
			{
				if (a[i] < a[lt]) { _swap(a[++m], a[i]); _swap(aIdx[m], aIdx[i]); }
			}
			_swap(a[lt], a[m]); _swap(aIdx[lt], aIdx[m]);
			sortArray(a, aIdx, lt, m); sortArray(a, aIdx, m + 1, rt);
		}
	};

	template <typename T, size_t N>
	constexpr auto  sortArray(const array<T, N>& a, const array<size_t, N>& b)
	{
		array<T, N> _a = a; array<size_t, N> _b = b;
		sortArray(_a, _b, 0, N);
		return tuple<array<T, N>, array<size_t, N>>{_a, _b};
	};

	template <typename T, size_t N>
	constexpr auto  sortArray(const tuple<array<T, N>, array<size_t, N>>& ta)
	{
		array<T, N> a = get<0>(ta);  array<size_t, N> b = get<1>(ta);
		sortArray(a, b, 0, N);
		return tuple<array<T, N>, array<size_t, N>>{a, b};
	};
#pragma endregion

#pragma region get keys array from base array
	template <auto F, class TF = GETVINPUTTEMPLATE(F), class T, size_t N,
		template <class, auto> class C, std::size_t... I>
	static constexpr auto getKeysIdx(const C<T, N>& a, index_sequence<I...>)
	{
		static_assert(is_same_v<TF::Ctin, T>, "getKeys:func input type isn't same base array val type");
		return array<TF::Ctres, N>{F(a[I]) ... };
	}

	//Make an array of objects with comparison operators from base array
	template <auto F, class T, size_t N, template <class, auto> class TA>
	constexpr auto getKeys(const TA<T, N>& a)
	{
		return  getKeysIdx<F>(a, make_index_sequence<N>{});
	}
#pragma endregion

#pragma region get array with indexes sequence	
	template<size_t N>
	static constexpr auto makeIdxsSeq() noexcept
	{
		return make_index_sequence<N>{};
	}

	template<size_t Beg, size_t... I>
	static constexpr auto makeIndexesSequenceFromIdx(index_sequence<I...>) noexcept
	{
		return index_sequence<(I + Beg)... >{};
	}

	template<size_t N, size_t Beg = 0>
	static constexpr auto makeIndexesSequenceFrom() noexcept
	{
		return makeIndexesSequenceFromIdx<Beg>(makeIdxsSeq<N>());
	}

	template<size_t... I>
	static constexpr auto makeIndexesSequenceTurnIdx(index_sequence<I...>) noexcept
	{
		return index_sequence<(sizeof...(I) - 1 - (I))... >{};
	}

	template<size_t N>
	static constexpr auto makeIndexesSequenceTurn() noexcept
	{
		return makeIndexesSequenceTurnIdx(makeIdxsSeq<N>());
	}

	template<size_t Beg, size_t... I>
	static constexpr auto makeIndexesSequenceFromTurnIdx(std::index_sequence<I...>) noexcept
	{
		return index_sequence<(sizeof...(I) - 1 + Beg - (I))... >{};
	}

	template<size_t N, size_t Beg = 0>
	static constexpr auto makeIndexesSequenceFromTurn() noexcept
	{
		return makeIndexesSequenceFromTurnIdx<Beg>(makeIdxsSeq<N>());
	}

	template<size_t... I>
	static constexpr auto makeArrayIdx(index_sequence<I...>) noexcept
	{
		return std::array<size_t, sizeof...(I)>{ {(I)...}};
	}

	template<size_t N>
	static constexpr auto makeIndexesArray() noexcept
	{
		return makeArrayIdx(makeIdxsSeq<N>());
	}

	template<size_t N, size_t Beg = 0>
	static constexpr auto makeIndexesArrayFrom() noexcept
	{
		return makeArrayIdx(makeIndexesSequenceFromIdx<Beg>(makeIdxsSeq<N>()));
	}

	//Make an array with indexes sequence
	template<size_t N>
	static constexpr auto makeIndexesArrayTurn() noexcept
	{
		return makeArrayIdx(makeIndexesSequenceTurnIdx(makeIdxsSeq<N>()));
	}

	template<size_t N, size_t Beg = 0>
	static constexpr auto makeIndexesArrayFromTurn() noexcept
	{
		return makeArrayIdx(makeIndexesSequenceFromTurnIdx<Beg>(makeIdxsSeq<N>()));
	}

	template <typename T, size_t N, const array<T, N> & a, size_t... I>
	static constexpr auto cutArr(index_sequence<I...>) noexcept
	{
		return array<T, sizeof...(I)>{a[I]...};
	}

	template <typename T, size_t N, const array<T, N> & a, size_t B = 0, size_t E = N - 1>
	static constexpr auto cutArr() noexcept
	{
		return cutArr<T, N, a>(makeIndexesSequenceFrom<E - B + 1, B>);
	}

	template <typename T, size_t N, size_t... I>
	static constexpr auto cutArr(const array<T, N>& a, index_sequence<I...>) noexcept
	{
		return array<T, sizeof...(I)>{a[I]...};
	}

	template <typename T, size_t N, size_t B = 0, size_t E = N - 1>
	static constexpr auto cutArr(const array<T, N>& a) noexcept
	{
		return cutArr<T, N>(a, makeIndexesSequenceFrom<E - B + 1, B>());
	}
	//------------------------------
	template <typename T, size_t N, const array<T, N> & a, size_t... I>
	static constexpr auto cutArrtoSecHelper(index_sequence<I...>) noexcept
	{
		return index_sequence<a[I]...>{};
	}

	template <typename T, size_t N, const array<T, N> & a, size_t B = 0, size_t E = N - 1>
	static constexpr auto cutArrtoSec() noexcept
	{
		return cutArrtoSecHelper<T, N, a>(makeIndexesSequenceFrom<E - B + 1, B>);
	}

	template <size_t N, size_t a, size_t... I, size_t... J>
	constexpr auto cutSecHelper(index_sequence<a, I ...>, index_sequence<J ...>) noexcept
	{
		if constexpr (sizeof...(J) == N)
			return index_sequence <J...>{};
		else
			return cutSecHelper<N>(index_sequence<I...>{}, index_sequence<J..., a>{});
	}

	template <size_t N, size_t... I>
	constexpr auto cutSec(index_sequence<I ...> B) noexcept
	{

		if constexpr (N == sizeof...(I)) return B;
		else
		{

			return cutSecHelper<N>(index_sequence<I...>{}, index_sequence<>{});

		}
	}
	template <size_t N, size_t a, size_t... I>
	constexpr auto cutSecB(index_sequence<a, I ...> B) noexcept
	{
		if constexpr (N == sizeof...(I) + 1) return B;
		else
		{
			if constexpr (sizeof...(I) == N)
				return index_sequence <I...>{};
			else
				return cutSecB<N>(index_sequence<I...>{});
		}
	}
#pragma endregion

#pragma region balance array
	template <typename T, size_t N>
	constexpr auto balanceArrTreeAftPrep(const array<T, N>& a, size_t E = N)
	{
		size_t segNum = 1, maxIdx = E, step = E, curIdx = 0, divRemn = 0,
			divRemnIdx = 0, i = 0, j = 0; array<T, N> res{}; auto tmp = a;
		while (step > 1)
		{
			i = step / 2; divRemn = maxIdx - ((maxIdx / segNum) * segNum);
			divRemnIdx = 0;
			while (i < maxIdx)
			{
				res[curIdx] = tmp[i];
				--maxIdx;
				for (j = i; j != maxIdx; ++j)
					tmp[j] = tmp[j + one];
				if (divRemnIdx < divRemn)
				{
					i += step;
					--divRemnIdx;
				}
				else
					i += step - 1;
				++curIdx;
			}
			segNum *= 2;
			step = maxIdx / segNum;
		}
		for (i = 0; i != maxIdx; ++i) { res[curIdx] = tmp[i]; curIdx++; }
		for (i = curIdx; i != N; ++i)  res[i] = a[i];

		return res;
	};

	template <typename T, size_t N>
	constexpr auto balanceArrTreeAftPrep(const pair<array<T, N>, size_t>& a)
	{
		return balanceArrTreeAftPrep(a.first, a.second);
	};

	template <typename T, size_t N>
	constexpr auto balanceArrTreeAftPrep
	(const tuple<array<T, N>, array<size_t, N>, size_t>& BB)
	{
		return balanceArrTreeAftPrep(std::get<1>(BB), std::get<2>(BB));
	};
#pragma endregion

#pragma region move Zero Back
	//Zero in begin once - after same placed to end
	template <typename T, size_t N>	constexpr auto  moveZeroBackIfSameInEnd
	(T Zero, const tuple<array<T, N>, array<size_t, N>, size_t>& BB)
	{
		auto A = get<0>(BB); auto B = get<1>(BB); size_t L = get<2>(BB); int i = 0;

		for (i = 0; i < L; i++)
			if (A[i] == Zero) break;

		if (i == L) return BB;

		size_t b = B[i];
		for (i++; i < L; i++) { B[i - 1] = B[i]; A[i - 1] = A[i]; }
		L--;
		B[L] = b;	A[L] = Zero;
		return tuple<array<T, N>, array<size_t, N>, size_t>{A, B, L};
	};

	template <typename T, size_t N>	constexpr auto  moveZeroBackAfterSorting
	(T Zero, const tuple<array<T, N>, array<size_t, N>, size_t>& BB)
	{
		auto A = get<0>(BB); auto B = get<1>(BB);
		size_t L = get<2>(BB);
		size_t i = 0;
		size_t j = 0;
		size_t tmp = 0;
		for (i = 0; i < L; i++)
			if (A[i] == Zero) break;
		for (j = i + 1; j < L; j++)
			if (A[j] != Zero) break;

		for (; j < L; j++, i++)
		{
			tmp = B[i];  B[i] = B[j]; B[j] = tmp; A[i] = A[j]; A[j] = Zero;
		}
		return   tuple<array<T, N>, array<size_t, N>, size_t>{A, B, i++};
	};
#pragma endregion

#pragma region move Repeat Back

	template <typename T, size_t N>
	constexpr bool isFirstValue(const array<T, N>& A, int I)
	{
		for (int i = 0; i != I; ++i) if (A[i] == A[I]) return false;
		return true;
	};

	template <typename T, size_t N>
	constexpr auto  moveRepeatBack(const tuple<array<T, N>, array<size_t, N>, size_t>& BB)
	{
		//const array<T, N>& A, const array<size_t, N>& B
		auto A = get<0>(BB); auto B = get<1>(BB);
		size_t L = get<2>(BB);

		array<size_t, N> s{}; int ii = 0, jj = L - 1; array<T, N> s1{};
		for (int i = 0; i < L; i++)
		{
			if (isFirstValue(A, i)) { s[ii] = B[i]; s1[ii] = A[i]; ii++; }
			else { s[jj] = B[i]; s1[jj] = A[i]; jj--; }
		}
		for (int i = L; i < N; i++)
		{
			s[i] = B[i]; s1[i] = A[i];
		}
		return tuple<array<T, N>, array<size_t, N>, size_t>{s1, s, ii};
	};

	template <typename T, size_t N>
	constexpr auto  moveRepeatBackForAll(const array<T, N>& A, const array<size_t, N>& B)
	{
		return moveRepeatBack(tuple<array<T, N>, array<size_t, N>, size_t>{A, B, N});
	};

	//get indexes unique data from array of index 
	template <typename T, size_t N>
	constexpr auto  getUniqueIdxs(const tuple<array<T, N>, array<size_t, N>, size_t>& BB)
	{
		//const array<T, N>& A, const array<size_t, N>& B
		auto A = get<0>(BB); auto B = get<1>(BB);
		size_t L = get<2>(BB);
		array<size_t, N> idxsRes{};
		array<size_t, N> dataIdxs{}; int ii = 0, jj = L - 1; array<T, N> data{};
		for (size_t i = 0; i < L; i++)
		{
			if (isFirstValue(A, i)) { idxsRes[ii] = i; dataIdxs[ii] = B[i]; data[ii] = A[i]; ii++; }
		}
		return tuple<array<T, N>, array<size_t, N>, array<size_t, N>, size_t>{data, dataIdxs, idxsRes, ii};
	};
#pragma endregion

#pragma region Main Array
	//get first index for max value in sorted array
	template <typename T, size_t N>
	constexpr auto getLLast(const tuple<array<T, N>, array<size_t, N>, size_t>& BB)
	{
		auto A = get<0>(BB); auto L = get<2>(BB); auto B = get<1>(BB);
		size_t i = L - 1;
		for (i = L - 2; i >= 0; i--)
		{
			if (A[L - 1] != A[i]) { i++; break; }
		}
		return i;
	};
	//move min/max to tail array index
	template <typename T, size_t N>
	constexpr auto  moveMinMaxBack
	(const tuple<array<T, N>, array<size_t, N>, size_t>& BB)
	{
		auto A = get<0>(BB); auto B = get<1>(BB); size_t L = get<2>(BB);

		size_t b = B[0]; T a = A[0];
		size_t b1 = B[L - 1]; T a1 = A[L - 1];
		for (int j = 1; j < L; j++)
		{
			B[j - 1] = B[j];
			A[j - 1] = A[j];
		}
		for (size_t j = L - 1; j < N - 1; j++)
		{
			B[j - 1] = B[j + 1];
			A[j - 1] = A[j + 1];
		}
		B[N - 2] = b; A[N - 2] = a;	B[N - 1] = b1; A[N - 1] = a1;
		L -= 2;
		return tuple<array<T, N>, array<size_t, N>, size_t>{A, B, L};
	};
	//Move first one item data to Previous Last Data - одно первое значение на предпоследнее
	template <size_t N>
	constexpr auto  moveMinPrevLast
	(const  array<size_t, N>& A, size_t L)
	{
		auto B = A;
		size_t b = B[0];
		for (int j = 1; j < L - 1; j++)	B[j - 1] = B[j];
		B[L - 2] = b;
		return B;
	};

	template <const auto & DO, auto V1>
	struct PR
	{
		static constexpr decltype(DO) first = DO;
		static constexpr decltype(V1) second = V1;
	};
#pragma endregion

	template <typename T, size_t N>
	constexpr auto isArrValSame(const array<T, N>& mA)
	{
		int i = 1;
		for (i = 1; i < N; i++) if (mA[i] != mA[0]) break;
		return  (i == N);
	}

	template <typename T, size_t N>
	constexpr auto makeZeroArr() {
		return  array <T, N>{T{}};

	}
	constexpr size_t st0 = 0;
	constexpr size_t st1 = 1;

	template <typename T, size_t N>
	constexpr auto  fillAfterBalanced
	(const array<T, N>& A, const  array<size_t, N>& B, const  array<size_t, N>& C, size_t EA, size_t EC)
	{
		array<size_t, N> s{};
		array<T, N> s1{};
		size_t  ii = 0;
		//int ii = 0, jj = L - 1; array<T, N> s1{};
		for (size_t i = 0; i < EC; i++)
		{
			s[ii] = B[C[i]]; s1[ii] = A[C[i]]; ii++;
			for (size_t j = C[i] + 1; j < EA; j++)
			{
				if (A[C[i]] == A[j]) {

					s[ii] = B[j]; s1[ii] = A[j]; ii++;
				}
				else {
					break;
				}
			}
		}
		return tuple{ s,ii };
	}

	template <typename T, size_t N>
	constexpr auto  addMinMaxtoLast
	(const array<T, N>& A, const  array<size_t, N>& B, const  array<size_t, N>& C, size_t EA, size_t EC)
	{
		array<size_t, N> s{};
		for (int i = 0; i < EC; i++) s[i] = C[i];

		size_t  i0 = EC;
		size_t i1 = EC;
		s[i1] = B[0]; i0++;
		for (size_t j = 1; j < EA; j++)
		{
			if (A[j] == A[0]) { s[i0] = B[j];  i0++; }
			else break;
		}
		size_t  i2 = i0;
		s[i0] = B[EA - 1];  i0++;
		for (size_t j = EA - 2; j >= 0; j--)
		{
			if (A[j] == A[EA - 1]) { s[i0] = B[j];  i0++; }
			else break;
		}
		return tuple{ s,i0, i1,i2 };
	}

	template <auto F, typename T, size_t N, template <class, auto> class TA>
	constexpr auto prepareBalancedIndexesLim(const TA<T, N>& mA) noexcept {
		auto wdww = cutArr<T, N, 1, 4>(mA);
		//auto wddww = cutArrtoSec<T, N, 1, 4>(mA);
		using outType = clearType_t<GETVINPUTTEMPLATE(F)::Tres>;
		//auto tmpInput = mA;
		auto tmpKeys = getKeys<F>(mA);
		auto tmpIndexes = makeIndexesArray<N>();
		constexpr auto tmpIdfes = makeZeroArr<T, N>();
		if (isArrValSame(tmpKeys)) {
			return tuple(tmpIndexes, st1, st1, st1);
		}
		sortArray(tmpKeys, tmpIndexes, 0, N);
		tuple SS{ tmpKeys, tmpIndexes ,N };
		auto tmp1 = moveZeroBackAfterSorting(F(T{}), SS);
		size_t NN = getLLast(tmp1);
		auto tmp2 = getUniqueIdxs(tmp1);
		auto tmp3 = balanceArrTreeAftPrep(get<2>(tmp2), get<3>(tmp2));
		auto tmp4 = fillAfterBalanced(get<0>(tmp1), get<1>(tmp1), tmp3, get<2>(tmp1), get<3>(tmp2));
		return tuple(get<0>(tmp4), get<1>(tmp4), st1, st1);//st1 - a plug from ancient times
	}

	template <auto F, typename T, size_t N, template <class, auto> class TA>
	constexpr auto prepareBalancedIndexes(const TA<T, N>& mA) noexcept {
		//auto w = cutArr<T, N, 1, 4>(mA);
		//auto w1 = cutArrtoSec<T, N, 1, 4>(mA);
		using outType = clearType_t<GETVINPUTTEMPLATE(F)::Tres>;
		auto tmpInput = mA;
		auto tmpKeys = getKeys<F>(mA);
		auto tmpIndexes = makeIndexesArray<N>();
		constexpr auto tmpIdfes = makeZeroArr<T, N>();
		if (isArrValSame(tmpKeys))	return tuple(tmpIndexes, st1, st1, st1);
		sortArray(tmpKeys, tmpIndexes, 0, N);
		tuple SS{ tmpKeys, tmpIndexes ,N };
		auto tmp1 = moveZeroBackAfterSorting(F(T{}), SS);
		size_t NN = getLLast(tmp1);
		auto tmp2 = getUniqueIdxs(tmp1);
		auto tmp3 = balanceArrTreeAftPrep(moveMinPrevLast(get<2>(tmp2), get<3>(tmp2)), get<3>(tmp2) - 2);
		auto tmp4 = fillAfterBalanced(get<0>(tmp1), get<1>(tmp1), tmp3, get<2>(tmp1), get<3>(tmp2) - 2);
		return addMinMaxtoLast(get<0>(tmp1), get<1>(tmp1), get<0>(tmp4), get<2>(tmp1), get<1>(tmp4));
	}

	template <auto F, typename T, size_t N, template <class, auto> class TA>
	constexpr auto prepareBalancedIndexesOld(const TA<T, N>& mA) noexcept
	{
		using outType = clearType_t<GETVINPUTTEMPLATE(F)::Tres>;
		auto tmpKeddys = mA;
		auto tmpKeys = getKeys<F>(mA);
		auto tmpIndexes = makeIndexesArray<N>();
		int i = 1;
		for (i = 1; i < N; i++) if (tmpKeys[i] != tmpKeys[0]) break;
		if (i == N) return tuple(tmpIndexes, (size_t)1, (size_t)1);
		//auto w = sortArray(tmpKeys, tmpIndexes);
		sortArray(tmpKeys, tmpIndexes, 0, N);

		tuple SS{ tmpKeys, tmpIndexes ,N };
		auto tmp1 = moveZeroBackAfterSorting(F(T{}), SS);
		auto tmp2 = moveRepeatBack(tmp1);

		auto tmp3 = balanceArrTreeAftPrep(moveMinPrevLast(get<1>(tmp2), get<2>(tmp2)), get<2>(tmp2) - 2);
			   
		auto tmp4 = moveRepeatBackForAll(tmpKeys, tmpIndexes);
		auto tmp5 = moveZeroBackIfSameInEnd(F(T{}), tmp4);

		/*if constexpr (NL<outType>) //bad for debugging
		{
			auto tmp6 = balanceArrTreeAftPrep(get<1>(tmp2), get<2>(tmp2));		
			return tuple(tmp6, get<2>(tmp2), get<2>(tmp1));
		}
		else
		{*/
			if (get<2>(tmp5) > 2) {
				auto tmp6 = balanceArrTreeAftPrep(moveMinPrevLast(get<1>(tmp2), get<2>(tmp2)), get<2>(tmp2) - 2);
				return tuple(tmp6, get<2>(tmp2) - 2, get<2>(tmp1));
			}
			else return tuple(get<1>(tmp4), get<2>(tmp4), get<2>(tmp1));//tmp1
		//}
	}

	template <auto F, typename T, size_t N, template <class, auto> class TA>
	constexpr auto prepareBalancedIndexesNumLimOld(const TA<T, N>& mA) noexcept
	{
		using outType = clearType_t<GETVINPUTTEMPLATE(F)::Tres>;
		auto tmpKeddys = mA;
		auto tmpKeys = getKeys<F>(mA);
		auto tmpIndexes = makeIndexesArray<N>();
		int i = 1;
		for (i = 1; i < N; i++) if (tmpKeys[i] != tmpKeys[0]) break;
		if (i == N) return tuple(tmpIndexes, (size_t)1, (size_t)1);
		auto wwb = sortArray(tmpKeys, tmpIndexes);
		sortArray(tmpKeys, tmpIndexes, 0, N);

		tuple SS{ tmpKeys, tmpIndexes ,N };
		auto tmp1 = moveZeroBackAfterSorting(F(T{}), SS);
		auto tmp2 = moveRepeatBack(tmp1);

		auto tmp3 = balanceArrTreeAftPrep(moveMinPrevLast(get<1>(tmp2), get<2>(tmp2)), get<2>(tmp2) - 2);
				
		auto tmp4 = moveRepeatBackForAll(tmpKeys, tmpIndexes);
		auto tmp5 = moveZeroBackIfSameInEnd(F(T{}), tmp4);

		//if constexpr (NL<outType>)
		{
			auto tmp6 = balanceArrTreeAftPrep(get<1>(tmp2), get<2>(tmp2));
			return tuple(tmp6, get<2>(tmp2), get<2>(tmp1));
		}	
	}
};

enum  TPLRes { Array = 0, PlaceNotZero, PlaceMin, PlaceMax };

template <class... F>
struct tuple_ :tuple<F...>
{
	template<class... V>
	tuple_(V... v) :tuple<V...>(v...) {};//decltype(v)...
	enum  TP { Array = 0, PlaceNotZero, PlaceMin, PlaceMax };
	template<auto R> auto get()
	{
		return std::get<R>(*this);
	}
};

template <class... F> auto Tuple_(F... v)
{
	return tuple_<F...>(v...);
};

auto Y1 = Tuple_(4, 8.0);
auto ewdfdbb = get<Y1.PlaceNotZero>(Y1);
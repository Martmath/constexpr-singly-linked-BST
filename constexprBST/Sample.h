#pragma once
#include "BaseSupport.h"
#pragma region SampleClass

struct Test1
{
	//const 
	int I_0 = BASENUM;
	//const 
	int I_1 = BASENUM;//IntData1
};

struct Test
{
	//const 
	int I_0 = BASENUM;
	//const 
	int I_1 = BASENUM;//IntData1
//const 
//	char I_S[3] = "ab";
	friend constexpr bool operator == (const Test& c1, const Test& c2);
	friend constexpr bool operator != (const Test& c1, const Test& c2);
	friend constexpr bool operator < (const Test& c1, const Test& c2);
	friend constexpr bool operator > (const Test& c1, const Test& c2);
	friend constexpr bool operator <= (const Test& c1, const Test& c2);
	friend constexpr bool operator >= (const Test& c1, const Test& c2);

	friend ostream& operator<< (ostream& os, const Test& a);
};

ostream& operator<<(ostream& os, const Test& dt)
{
	os <<'{' << dt.I_0 <<',' << dt.I_1 << '}';
	return os;
}

static constexpr bool operator>= (const Test& c1, const Test& c2)
{
	return (c1.I_0 >= c2.I_0);
}

static constexpr bool operator<= (const Test& c1, const Test& c2)
{
	return (c1.I_0 <= c2.I_0);
}

static constexpr bool operator< (const Test& c1, const Test& c2)
{
	return (c1.I_0 < c2.I_0);
}

static constexpr bool operator> (const Test& c1, const Test& c2)
{
	return (c1.I_0 > c2.I_0);
}

static constexpr bool operator== (const Test& c1, const Test& c2)
{
	return (c1.I_0 == c2.I_0);
}

static constexpr bool operator!= (const Test& c1, const Test& c2)
{
	return !(c1 == c2);
}

struct A {
	int a; int b; int c;
#ifdef SpaceShipOperator
	auto operator<=>(const A& rhs) const { return (a <=> rhs.a); };
#endif
#if 0
	// friend bool operator ==(const A&, const A&);
	bool operator  ==(const A& rhs) const { return (a == rhs.a); }
	bool operator  <(const A& rhs) const { return (a < rhs.a); }
	bool operator  >(const A& rhs) const { return (a > rhs.a); }
	bool operator  >=(const A& rhs) const { return (a >= rhs.a); }
#endif
};
#if 0
bool operator==(const A& l, const A& r) { return (l.a == r.a); }
#endif 

#pragma endregion

constexpr //auto& 
TgetKeyFunc<Test, int>&
getKeyFnc = getKeyFunc0 < int, Test>;

constexpr const int getKeyFn(const Test& t) { return t.I_0; };

constexpr const bool getKey2_7_14(const Test& t) { return (t.I_0 == 2) || (t.I_0 == 7) || (t.I_0 == 14); };

constexpr const bool getKeyAll(const Test& t) { return true; };
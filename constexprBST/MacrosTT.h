


namespace CP
{
 
#ifdef WW_
	#define WW__(a,b) a
#else
	#define WW__(a,b) b
#endif

#if defined(XX_) && defined(YY_)
	#define XX_YY__(a,b,c,d) a
#elif defined(XX_)
	#define XX_YY__(a,b,c,d) b
#elif defined(YY_)
	#define XX_YY__(a,b,c,d) c
#else
	#define XX_YY__(a,b,c,d) d
#endif


#define IFDEF2(m,a,b) m##_(a,b)//ifdf  
#define IFDEF4(m,n,a,b,c,d) m##n##_(a,b,c,d) 
} 

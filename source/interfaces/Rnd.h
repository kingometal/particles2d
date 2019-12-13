//=========================================================================
//	Random number generator	(Algorithm by R. M. Ziff)
//=========================================================================
#ifndef RND_H
#define RND_H

#include <stdlib.h>
using namespace std;

const int Random_A = 471;
const int Random_B = 1586;
const int Random_C = 6988;
const int Random_D = 9689;
const int Random_M = 16383;
const unsigned int Random_Max = 2147483648U;
int Random_nd, Random_ra[Random_M+1];

void InitRandom (int randomize=1)
{   srandom((unsigned int)randomize*time(0));
    for (int i=0; i<=Random_M; ++i) Random_ra[i] = random();
}

#define RandomInteger (++Random_nd, Random_ra[Random_nd & Random_M] = \
	Random_ra[(Random_nd-Random_A) & Random_M] ^ \
        Random_ra[(Random_nd-Random_B) & Random_M] ^ \
	Random_ra[(Random_nd-Random_C) & Random_M] ^ \
	Random_ra[(Random_nd-Random_D) & Random_M])

inline double rnd(void) {
    return (double)RandomInteger/Random_Max;
}


//-------------------Auxiliary random number routines---------------------

inline double rnd (double max) 			{
    return max*rnd();
}
inline double rnd (double min, double max) 	{
    return min+(max-min)*rnd();
}
inline int rndint (int num=2) 			{
    return (int)(num*rnd());
}
inline int rndint (int min, int max)		{
    return min+rndint(max-min+1);
}

#endif // RND_H

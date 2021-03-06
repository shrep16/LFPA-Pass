/*
 * Simple program to test flow-sensitive analysis.
 * Author: Sen Ye
 * Date: 08/11/2013
 */

//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

	int **p, **q;
	int *x, *y;
	int x0, y0;
int main() {
	p = &x;
	q = &y;
	*p = &x0;
	*q = &y0;
	mustPointsTo(*p,x0);
	mustPointsTo(*q,y0);
	x0 += **p;
	//NOALIAS(*p, *q);
	*p = *q;

	mustPointsTo(*p,y0);
	x0 += **p;
	//MUSTALIAS(*p, *q);
	return x0;
}

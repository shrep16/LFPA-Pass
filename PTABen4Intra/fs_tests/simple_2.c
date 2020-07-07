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


	int *p, *q, *r;
	int x, y, z;
int main() {
	p = &x;
	q = &y;
	r = &z;
	mustPointsTo(p,x);
	mustPointsTo(q,y);
	//NOALIAS(p, q);
	x = *p;
	p = q;
	mustPointsTo(p,y);
	x += *p;
	//MUSTALIAS(p, q);
	p = r;
	mustPointsTo(p,z);
	x+=*p;

	//NOALIAS(p, q);
	return x;
}

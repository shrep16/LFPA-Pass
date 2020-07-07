/*
 * Branches for testing flow-sensitive analysis.
 * Author: Sen Ye
 * Date: 08/11/2013
 */

//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

	int *p, *q;
	int x, y;
int main() {
	if (x)
		p = &x;
	else
		p = &y;
	q = &y;
	mustPointsTo(p,x);
	mustPointsTo(p,y);
	mustPointsTo(q,y);
	mayPointsTo(p,x);
	mayPointsTo(p,y);
	//MAYALIAS(p, q);
	return *p+*q;
}

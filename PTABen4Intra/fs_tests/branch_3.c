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


	int **p, **q;
	int *x, *y;
	int x0, y0;
int main() {
	if (x0) {
		p = &x;
		q = &y;
		mustPointsTo(p,x);
		mustPointsTo(q,y);
		//NOALIAS(p, q);
	}
	else {
		p = &y;
		q = &x;
		mustPointsTo(p,y);
		mustPointsTo(q,x);
		//NOALIAS(p, q);
	}
	mustPointsTo(p,x);
	mayPointsTo(p,x);
	mustPointsTo(q,y);
	mayPointsTo(q,y);
	//MAYALIAS(p, q);
	return **p+**q;
}

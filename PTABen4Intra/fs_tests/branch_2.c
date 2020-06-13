/*
 * Branches for testing flow-sensitive analysis.
 * Author: Sen Ye
 * Date: 08/11/2013
 */

//#include "aliascheck.h"
#include "myHeader.h"

	int x, y;
	int *p, *q;
int main() {
	q = &y;
	if (x) {
		p = &x;
		mustPointsTo(p,x);
		x = *p;
		//NOALIAS(p,q);
	}
	else {
		p = &y;
		mustPointsTo(p,y);
		x =*p;
		//MUSTALIAS(p,q);
	}
	mustPointsTo(q,y);
	x +=*q;

	return x;
}

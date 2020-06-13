/*
 * Alias due to lack of path-sensitivity.
 * Author: Sen Ye
 * Date: 06/09/2013
 */
//#include "aliascheck.h"
#include "myHeader.h"

	int *p, *q;
	int a, b, c;
int main()
{
	if (c) {
		p = &a;
		q = &b;
	}
	else {
		p = &b;
		q = &c;
	}
	mayPointsTo(p,a);
	mayPointsTo(p,b);
	mayPointsTo(q,b);
	mustPointsTo(q,c);
//	MAYALIAS(p,q);
	return *p+*q;
}

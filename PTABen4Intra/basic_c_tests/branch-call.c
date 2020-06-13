/*
 * Alias due to lack of context-sensitivity.
 * Author: Sen Ye
 * Date: 06/09/2013
 */
//#include "aliascheck.h"
#include "myHeader.h"

/*void foo(int *m, int *n)
{
	MAYALIAS(m,n);
	int x, y;
	x = *n;
	y = *m;
	*m = x;
	*n = y;
}*/

	int *p, *q;
	int a, b, c;
int main()
{
	if (c) {
		p = &a;
		q = &b;
		//foo(p,q);
		mustPointsTo(p,a);
		mustPointsTo(q,b);
	}
	else {
		p = &b;
		q = &c;
		//foo(p,q);
		mustPointsTo(p,b);
		mustPointsTo(q,c);
	}
	return *p+*q;
}

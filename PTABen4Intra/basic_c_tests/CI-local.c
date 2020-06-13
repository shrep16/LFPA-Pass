/*
 * Alias due to context-insensitive
 * Author: Sen Ye
 * Date: 06/09/2013
 */
//#include "aliascheck.h"
#include "myHeader.h"

/*void foo(int *m, int *n)
{
	MAYALIAS(m,n);
}*/

	int *p, *q;
	int a,b;
int main()
{
	if (a) {
		p = &a;
		q = &b;
		mustPointsTo(p,a);
		mustPointsTo(q,b);
		//foo(p,q);
	}
	else {
		p = &b;
		q = &a;
		mustPointsTo(p,b);
		mustPointsTo(q,a);
		//foo(p,q);
	}
	mustPointsTo(p,a);
	mustPointsTo(p,b);
	mustPointsTo(q,a);
	mustPointsTo(p,b);

	return *p+*q;
}

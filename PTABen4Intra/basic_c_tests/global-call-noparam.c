/*
 * Global variables test.
 * Author: Sen Ye
 * Date: 03/05/2014
 */
//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2


int *p;
int *q;
int c;

/*void foo() {
	MAYALIAS(p, q);
}

void bar() {
	q = &c;
}
*/
	int a, b;
int main() {
	doesNotPointsTo(p,a); //will not print not live anymore
	c = *p;
	p = &a;
	mustPointsTo(p,a);
	q = p;
	mustPointsTo(q,a);
	p = &c;
	mustPointsTo(p,c);
	c+= *p+*q;
	return c;
}

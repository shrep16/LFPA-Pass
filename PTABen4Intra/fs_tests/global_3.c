/*
 * Global pointer in flow-sensitive analysis.
 * Author: Sen Ye
 * Date: 08/11/2013
 */

//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2


int **pp, **qq;
int *p, *q;
int x;

/*void foo() {
	*pp = &x;
}

void bar() {
	qq = &q;
	q = &x;
}*/

int main() {
	pp = &p;
	*pp = &x;
	qq = &q;
	q = &x;
	mustPointsTo(*pp,x);
	mustPointsTo(*qq,x);
	//foo();
	//bar();
	//MUSTALIAS(*pp, *qq);
	return **pp+**qq;
}

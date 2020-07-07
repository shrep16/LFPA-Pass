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
int x, y;

/*void foo() {
	pp = &p;
	p = &x;
	qq = &q;
	q = &y;
	NOALIAS(*pp, *qq);
}

void bar() {
	qq = &q;
	q = &x;
}*/

int main() {
	//foo();
	pp = &p;
	p = &x;
	qq = &q;
	q = &y;
	mustPointsTo(*qq,y);
	x = **qq;
	qq = &q;
	q = &x;
	mustPointsTo(*pp,x);
	mustPointsTo(*qq,x);
	x += **pp+**qq;
	//bar();
	//MUSTALIAS(*pp, *qq);
	return x;
}

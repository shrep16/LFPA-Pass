/*
 * Global pointer in flow-sensitive analysis.
 * Author: Sen Ye
 * Date: 08/11/2013
 */

//#include "aliascheck.h"
#include "myHeader.h"

int **pp, **qq;
int *p, *q;
int x;

/*void foo() {
	pp = &p;
	p = &x;
}

void bar() {
	qq = &q;
	q = &x;
}*/

int main() {
	pp = &p;
	p = &x;
	qq = &q;
	q = &x;
	mustPointsTo(*pp,x);
	mustPointsTo(*qq,x);
	//foo();
	//bar();
	//MUSTALIAS(*pp, *qq);
	return **pp+**qq;
}

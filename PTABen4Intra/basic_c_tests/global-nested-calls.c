/*
 * Global variable
 * Author: Sen Ye
 * Date: 13/10/2013
 * Description: Initialise global variables in callee and check alias
 *		in caller.
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
	q=&x;
	mustPointsTo(*pp,x);
	mustPointsTo(*qq,x);
	mustPointsTo(*pp,*q);
	mustPointsTo(*qq,*p);
	//MAYALIAS(*pp, *qq);
	//foo();
	//bar();
	return **pp+**qq;
}

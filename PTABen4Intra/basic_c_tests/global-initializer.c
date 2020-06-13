/*
 * Global variable
 * Author: Sen Ye
 * Date: 13/10/2013
 * Description: initialise global variables when declared
 *		and check alias in main function.
 */
//#include "aliascheck.h"
#include "myHeader.h"
int x;
int *p, *q;
int **pp = &p;
int **qq = &q;

/*void foo() {
	p = &x;
}

void bar() {
	q = &x;
}*/

int main() {
	mustPointsTo(pp,p);
	mustPointsTo(qq,q);
	//MAYALIAS(*pp, *qq);
//	foo();
//	bar();

	p = &x;
	q = &x;
	mustPointsTo(p,x);
	mustPointsTo(q,x);


	mustPointsTo(*pp,x);
	mustPointsTo(*qq,x);


	x = *p + *q + **pp + **qq;
	return x;
}

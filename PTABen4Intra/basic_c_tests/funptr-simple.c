/*
 * Function pointer.
 * Author: Sen Ye
 * Date: 10/10/2013
 */
//#include "aliascheck.h"

/*void f(int *p, int *q) {
	// if function pointer solved correctly,
	// p and q will alias due to CS1
	MAYALIAS(p,q);
}*/

//void (*fptr)(int*,int*);
#include "myHeader.h"

	int *m, *n;
	int x, y;
int main() {
	if (x) {
		m = &x, n = &x;
//		fptr = f;
//		fptr(m,n); // CS1
	}
	else {
		m = &x; n = &y;
//		f(m,n); // CS2
	}

	mustPointsTo(m,x);
	mayPointsTo(n,x);
	mayPointsTo(n,y);
	return *m+*n;
}

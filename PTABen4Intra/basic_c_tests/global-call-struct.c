/*
 * Global variables test.
 * Author: Sen Ye
 * Date: 07/05/2014
 */
//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

struct MyStruct {
	char f0[20];
	int f1;
	int* f2;
};

int x, y;
/*struct MyStruct global = {"abcdefg", 20, &x};

void foo(int** pp, int** qq) {
	*pp = &x;
	*qq = &y;
}

void bar(int** pp, int** qq) {
	*pp = &x;
	*qq = &x;
}*/

	int *p, *q;
	int **pp, **qq;
int main() {
	pp = &p;
	qq = &q;
	*pp = &x;
	*qq = &x;
	mustPointsTo(*pp,x);
	mustPointsTo(*qq,x);
	mustPointsTo(pp,*pp);
	mustPointsTo(pp,p);
	mustPointsTo(qq,q);
	//bar(pp,qq);
	//MAYALIAS(p, q);
	//MAYALIAS(global.f2, *qq);
	return **pp+**qq;
}

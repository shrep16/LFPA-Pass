//#include "aliascheck.h"
#include "myHeader.h"

int* g;

/*void foo(int **p, int *q){
	g = q;
	*p = g;
}*/

	int **a,*b,*a1,b1;
int main(){
	a = &a1;
	b = &b1;
	mustPointsTo(b,b1);
	g = b;
	*a = g;
	mustPointsTo(*a,b1);
	mustPointsTo(a1,b1);
	return **a;
	//foo(a,b);
	//MUSTALIAS(a1,b);
}

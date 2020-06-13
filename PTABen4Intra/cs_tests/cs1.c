//#include "aliascheck.h"
#include "myHeader.h"
/*void foo(int **x, int **y){

	x = y;
}*/

	int *a, *b;
	int a1, b1;
	int **x, **y;
int main(){

	a = &a1;
	b = &b1;
	//foo
	x = &a;
	mustPointsTo(x,a); //will fail - not live
	y = &b;
	x = y;
	mustPointsTo(a,a1);
	mustPointsTo(b,b1);
	mustPointsTo(y,b);
	mustPointsTo(x,b);
	



	//foo(a,b);
    //NOALIAS(a,b);
    return **x+**y+*a+*b;
}

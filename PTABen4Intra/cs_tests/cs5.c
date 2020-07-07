//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

int** g;

/*void foo(int **p, int *q){
	g = p;
	*g = q;

}*/

	int **a,*b,*a1,b1;
int main(){
	a = &a1;
	b = &b1;
	g = a;
	*g = b;
	mustPointsTo(a1,b1);
	mustPointsTo(b,b1);
	return **g+*b;
	//foo(a,b);
	//MUSTALIAS(a1,b);
}

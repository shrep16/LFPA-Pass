//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2


/*void foo(int**a, int*b){

	*a = b;

}*/


	int *p,q,*x,y;
int main(){
	int **a,*b;
	a = &p;
	b = &q;
	*a = b;
	mustPointsTo(p,q);
	//foo(&p,&q);
    //MUSTALIAS(p,&q);

	//foo(&x,&y);
	a = &x;
	b = &y;
	*a = b;
	mustPointsTo(x,y);
	doesNotPointsTo(x,q);
	doesNotPointsTo(p,y);
    //MUSTALIAS(x,&y);
    //NOALIAS(x,&q);
    //NOALIAS(p,&y);

	*p = 100;
	return *x+*p;

}

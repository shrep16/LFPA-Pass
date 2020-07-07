//#include "aliascheck.h"

#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

//void foo(int**, int*);
	int **x, *y;
	int  *c, *d,e,f;
int main(){
	if(x) { x =&c; y =&e;}
	else { x= &d; y = &f;}
	//foo(x,y);
	int **p = x;
	int *q = y;
	*p = q;
	mayPointsTo(c,e);
	mayPointsTo(c,f);
	mayPointsTo(d,e);
	mayPointsTo(d,f);
	return **x;

    /*MUSTALIAS(c,&e);
    MUSTALIAS(d,&f);
    NOALIAS(c,&f);
    NOALIAS(d,&e);*/
	
}

/*void foo(int **p, int *q){
	*p = q;
}*/

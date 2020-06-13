//#include "aliascheck.h"
#include "myHeader.h"
int obj;
	int **x, **y;
	int *a, *b, *c, *d,*e;
//void foo(int**, int**);
int main(){
	x=&a; y =&b;
       *x = &obj;
	mustPointsTo(a,obj);
 	mustPointsTo(*x,obj);
	mustPointsTo(y,b);
	doesNotPointsTo(b,obj);
	obj += **x+**y;	
	//foo(x,y);
    //MUSTALIAS(a,&obj);
	x = &c;
	//foo(x,y);
	*x = &obj;
	mustPointsTo(c,obj);
	obj += **x;
	return obj;
    //MUSTALIAS(c,&obj);
	
}

/*void foo(int **p, int **q){
	*p = &obj;
}*/

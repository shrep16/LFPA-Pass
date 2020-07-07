//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

//void foo(int*,int*);
	int*x, *y;
	int a,b, c;
int main(){
	if(c)x=&a; 
	else x=&b;
	mustPointsTo(x,a);//fail
	mustPointsTo(x,b);//fail
	mayPointsTo(x,a);//pass
	c = *x;
    //MAYALIAS(x,&a);
    //MAYALIAS(x,&b);
	//foo(x,y);
	x = &c;
	mustPointsTo(x,c);//pass
	doesNotPointsTo(x,a);//pass
	doesNotPointsTo(x,a);//pass
	c+= *x;
	return c;
	//foo(x,y);
    //MUSTALIAS(x,&c);
    //NOALIAS(x,&a);
    //NOALIAS(x,&b);
	
}

/*void foo(int *p,int*q){
	*p = 100;
}*/

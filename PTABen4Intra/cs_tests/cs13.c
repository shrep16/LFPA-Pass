//#include "aliascheck.h"
//void foo(int*);
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

int ss = 20;
    int *a,*b,obj,t;
int main(){
	static int *k = &ss;
	//k = &ss;
	mustPointsTo(k,ss);
	//obj = *k;
	obj = *k;
    a=&obj;
    //foo(a);
    k = a;

    //*a=200;
    b=&t;
    //foo(b);
    k = b;
    doesNotPointsTo(b,ss);
    doesNotPointsTo(a,ss);
    obj += *a+*b;
    return obj;
	//return obj;

    //NOALIAS(a,&ss);   
    //NOALIAS(b,&ss);   
}

/*void foo(int* x){
     static int* k =&ss;
    printf("%d\n", *k);
    *k=200;
    k=x;
     *x= 100;
}*/

//#include "aliascheck.h"
//void foo(int*);
#include "myHeader.h"
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

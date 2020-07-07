//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

int obj1,obj2;
/*void foo(int **p, int **q){

	*p = &obj1;
	*q = &obj2;

}*/

	int **a,**b,*x,*y,*z;
int main(){

	if(a){
		a = &x;
		b = &y;
	}
	else{
		a = &z;
		b = &z;
	}
	//foo(a,b);
	*a = &obj1;
	*b = &obj2;
	mustPointsTo(x,obj1);
	mustPointsTo(z,obj1);
	mustPointsTo(y,obj2);
	mustPointsTo(z,obj2);
	return **a+**b;
    //MAYALIAS(x,&obj1);
    //MAYALIAS(z,&obj1);
    //MAYALIAS(y,&obj2);
    //MAYALIAS(z,&obj2);
    //NOALIAS(x,&obj2);

}

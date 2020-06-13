//#include "aliascheck.h"
#include "myHeader.h"
int x,y,*q,*f,*e,d;

/*void foo(int **p){
	f = &x;
	if(x){
		p = &e;
		f = &y;
	}

	*p = f;
    MAYALIAS(q,&x);
    MAYALIAS(e,&y);
    NOALIAS(q,&y);
    NOALIAS(q,&d);
    NOALIAS(e,&x);
}*/



	int **a,c;
int main(){
	a = &q; f = &d;
	f = &x;
if(x) {
	a = &e;
	f = &y;
}
*a = f;
mayPointsTo(e,x);
mayPointsTo(e,y);
doesNotPointsTo(e,d);
mayPointsTo(q,x);
mayPointsTo(q,y);
return **a;
	//foo(a);
}
//*a = f;



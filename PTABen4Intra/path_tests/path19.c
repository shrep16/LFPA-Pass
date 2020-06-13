//#include "aliascheck.h"
#include "myHeader.h"

int y,*q,*r,*f,*e,c,d;

/*void foo(int **p){

    if(y){
		p = &e;
		f = &y;
	}

	*p = f;
    MAYALIAS(q,&d);
    MAYALIAS(r,&c);
    MAYALIAS(e,&y);
    NOALIAS(q,&y);
    NOALIAS(r,&y);
}*/



	int **a,**b;
int main(){
    e = 0;
	a = &q; f = &d;	
//	foo(a);
	if(y) {
	
	a = &e;
	f = &y;		
	}
	*a = f;
    b = &r; f = &c;
    if(y) {
    b = &e;
    f = &y;
    }
    *b = f;
    mayPointsTo(e,d);
    mayPointsTo(e,y);
    //mayPointsTo(e,c);
    mayPointsTo(q,d);
    mayPointsTo(q,y);
    mayPointsTo(e,c);
    //mayPointsTo(e,y);
    mayPointsTo(r,c);
    mayPointsTo(r,y);
    doesNotPointsTo(r,d);
    doesNotPointsTo(q,c);
	return **a+**b;

//    foo(b);
    /*NOALIAS(r,&d);
    NOALIAS(q,&c);
    NOALIAS(e,&d);
    NOALIAS(e,&c);*/
}


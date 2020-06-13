//#include "aliascheck.h"
#include "myHeader.h"

    int **p,*q;
    int *a,*b,c,d;
int main(){
    if(a){
        p = &a;
        q = &c;
    }
    else if(b) {
        p = &b;
        q = &d;
    }
    else{
	    c=10;
      //printf("dummy branch\n");
    }
    *p = q;
    mustPointsTo(p,a);
    mustPointsTo(p,b);
    mustPointsTo(q,c);
    mustPointsTo(q,d);
    mustPointsTo(a,d);
    mustPointsTo(a,c);
    mustPointsTo(b,c);
    mustPointsTo(b,d);
    mayPointsTo(b,d);//pass
    mayPointsTo(b,c);//pass
    mayPointsTo(a,c);//pass
    mayPointsTo(a,d);//pass
    return **p+*q;

    /*MAYALIAS(a,&c);
    MAYALIAS(b,&d);
    NOALIAS(a,&d);
    NOALIAS(b,&c);*/
}

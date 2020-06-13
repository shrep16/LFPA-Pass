//#include "aliascheck.h"
#include "myHeader.h"

int **p,*q;

    int *a,*b,c,d;
int main(){

    p = &a;
    if(p){
        if(c){
            q = &c;
        }
    }
    else{
        p = &b;
        q = &d;
    }
    if(d){
        *p = q;
    }
    mayPointsTo(a,c);
    mayPointsTo(a,d);
    mayPointsTo(b,c);
    mayPointsTo(b,d);
    return *a+*b;
    /*MAYALIAS(a,&c);
    MAYALIAS(b,&d);
    NOALIAS(a,&d);
    NOALIAS(b,&c);*/


}

/*#include "aliascheck.h"

void foo(int** s);
void bar(int** s);*/
#include "myHeader.h"

int k;
    int **p,*q;
    int *b,*c,e;
int main(){

    if(e){
        p = &b;
	q = &k;
        //foo(&q);
    }
    else{
        p = &c;
        q = &e;
    }

    *p = q;
    mayPointsTo(b,k);
    mayPointsTo(b,e);
    mayPointsTo(c,k);
    mayPointsTo(c,e);
    return **p+*q;
    /*MAYALIAS(b,&k);
    MAYALIAS(c,&e);
    NOALIAS(b,&e);
    NOALIAS(c,&k);*/
}

/*void foo(int**x){
    bar(x);
}

void bar(int**s){
    *s = &k;
}*/

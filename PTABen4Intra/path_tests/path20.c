//#include "aliascheck.h"
#define TYPE1 void *
#define TYPE2 void *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2


int**a,**b, *f,*g,r,w,q,*obj,k;
int main(){

    f = &k;
    if(a){
        a = &f;
        f = &r;
    }
    else{
        a = &g;
        g = &w;
    }
  b = a;

    *a = &q;
    obj = *b;
    mustPointsTo(g,w);
    mayPointsTo(g,w);
    mayPointsTo(f,k);
    mayPointsTo(f,r);
    mayPointsTo(obj,r);
    mayPointsTo(obj,k);
    return *obj+*f+*g;
    /*MUSTALIAS(obj,&q);
    NOALIAS(obj,&r);
    NOALIAS(obj,&w);*/

}

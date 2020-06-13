//#include "aliascheck.h"
#include "myHeader.h"

int ***p,**a,**b,*q,*r,*f,v,z,*g,f1; 
int main(){

    p = &a;
    a = b = &f;
    f = &f1;
    q = &v;

    if(a){
        f = &z;
        *p = &g;
        b = *p;
    }
    else{


    }

    *a = q;
    r = *b;
    mayPointsTo(r,v);
    mayPointsTo(r,f1);
    mayPointsTo(r,z);
    mayPointsTo(a,g);
    mayPointsTo(a,f);
    mayPointsTo(g,v);
    return *r+*g+**a;
//    MUSTALIAS(r,q);
 //   NOALIAS(r,&z);
   // NOALIAS(r,&f1);
}

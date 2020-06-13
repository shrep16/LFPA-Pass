//#include "aliascheck.h"
#include "myHeader.h"
    int **p, *q, **w,*v, *a,a1,q1;
int main(){
    a = &a1;
    p = &a;
    q = &q1;
    w = 0;
    if(p)
        *p = q;
    else
        w = &a;

    v = *w;
    mayPointsTo(v,a1);
    mayPointsTo(v,q1);
    return *v;
    //NOALIAS(v,&q1);
    //MAYALIAS(v,&a1);

}

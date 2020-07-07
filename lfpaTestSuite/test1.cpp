#define TYPE1 int *
#define TYPE2 int
#include "myHeader.h"
#undef TYPE1
#undef TYPE2

#define TYPE1 int **
#define TYPE2 int *
#include "myHeader.h"
#undef TYPE1
#undef TYPE2


int **y, *v, **z, *u, *x, w, b;


int main(){
    y = &v;
    z = &u;
    x = &w;
    v = &b;
    mayPointsTo(y,v);
    mustPointsTo(y,v);
    mustPointsTo(x,w);
    while(w>0)
    {
       
        u = *y;
    }
    mustPointsTo(x,w);
    mayPointsTo(x,w);
    //func(y);
    return (*x+*u);
}

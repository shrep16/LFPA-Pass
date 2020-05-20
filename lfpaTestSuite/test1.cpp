#include "myHeader.h"

int **y, *v, **z, *u, *x, w, b;


int main(){
    y = &v;
    z = &u;
    x = &w;
    v = &b;
    mayPointsTo(y,v);
    mustPointsTo(v,w);
    while(w>0)
    {
       
        u = *y;
    }
    mayPointsTo(x,b);
    //func(y);
    return *x;
}

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
    mustPointsTo(x,w);
    mayPointsTo(x,w);
    //func(y);
    return (*x+*u);
}

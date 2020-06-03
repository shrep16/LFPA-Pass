#include "myHeader.h"


int ***m,**n,*z,*y,z1,y1;
int *p, *s,*r,*w,*q,*x;

int main(){

    m=&n;
    n=&z;
    *m=&y;
    doesNotPointsTo(n,z);

    z=&z1;
    y=&y1;
    ***m=10;
    mustPointsTo(m,n);
    mustPointsTo(n,y);
    mustPointsTo(y,y1);
    doesNotPointsTo(z,z1);
    z=**m;
    mustPointsTo(z,y1);
    return *z;
}

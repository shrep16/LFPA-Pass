#include "myHeader.h"

int *p, *s,*r,*w,*q,*x;
int ***m,**n,*z,*y,z1,y1;

int main(){

    m=&n;
    n=&z;
    *m=&y;
    z=&z1;
    y=&y1;
    mustPointsTo(m,n);
    mustPointsTo(z,z1);
    mustPointsTo(y,y1);
    mustPointsTo(n,y);
    doesNotPointsTo(n,z);
    ***m=10;
    z=**m;
    mustPointsTo(z,y1);
	
    return *z;
}
